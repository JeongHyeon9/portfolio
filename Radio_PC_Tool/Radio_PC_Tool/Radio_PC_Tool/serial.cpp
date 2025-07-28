#include "serial.h"

HANDLE hSerial;

bool find_CDC_COMport_by_VID(const TCHAR* targetVID, TCHAR* outPort, DWORD outSize)
{
    HKEY hUSBKey;
    HKEY hVIDKey;
    HKEY hDeviceParams;
    TCHAR full_path[512];
    DWORD index = 0;
    TCHAR subKeyName[256];
    DWORD subKeySize;
    DWORD subIndex = 0;
    TCHAR instanceKey[256];
    DWORD instanceSize;
    DWORD type = 0;
    DWORD size = outSize;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Enum\\USB"), 0, KEY_READ, &hUSBKey) != ERROR_SUCCESS)
    {
        return false;
    }

    while (true)
    {
        subKeySize = sizeof(subKeyName) / sizeof(TCHAR);
        if (RegEnumKeyEx(hUSBKey, index, subKeyName, &subKeySize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
            break;

        if (_tcsstr(subKeyName, targetVID))
        {
            if (RegOpenKeyEx(hUSBKey, subKeyName, 0, KEY_READ, &hVIDKey) == ERROR_SUCCESS)
            {
                while (true)
                {
                    instanceSize = sizeof(instanceKey) / sizeof(TCHAR);
                    if (RegEnumKeyEx(hVIDKey, subIndex, instanceKey, &instanceSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                    {
                        break;
                    }
                    _stprintf_s(full_path, _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\%s\\%s\\Device Parameters"), subKeyName, instanceKey);

                    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, full_path, 0, KEY_READ, &hDeviceParams) == ERROR_SUCCESS)
                    {
                        if (RegQueryValueEx(hDeviceParams, _T("PortName"), NULL, &type, (LPBYTE)outPort, &size) == ERROR_SUCCESS)
                        {
                            RegCloseKey(hDeviceParams);
                            RegCloseKey(hVIDKey);
                            RegCloseKey(hUSBKey);
                            return true;  
                        }
                        RegCloseKey(hDeviceParams);
                    }

                    ++subIndex;
                }

                RegCloseKey(hVIDKey);
            }
        }

        ++index;
    }

    RegCloseKey(hUSBKey);
    return false;
}

bool open_serial_port(TCHAR* portName) 
{
    TCHAR full_name[64];

    _stprintf_s(full_name, _T("\\\\.\\%s"), portName);
    hSerial = CreateFileW((LPCWSTR)portName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) 
    {
        return false;
    }

    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb);
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    SetCommState(hSerial, &dcb);

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    return true;
}

void write_to_serial(uint8_t* data, size_t length) 
{
    DWORD bytes_written;
    WriteFile(hSerial, data, (DWORD)length, &bytes_written, nullptr);
}

void read_from_serial() 
{
    uint8_t buffer[100] = { 0, };
    uint32_t sn;
    char str[32];
    DWORD bytes_read;

    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, nullptr))
    {
        buffer[bytes_read] = 0;
        if (buffer[0] == PACKET_HEADER && buffer[1] == PACKET_START && buffer[bytes_read - 2] == PACKET_END && buffer[bytes_read - 1] == PACKET_TAIL)
        {
            sn = ((uint32_t)buffer[2] << 24) | ((uint32_t)buffer[3] << 16) | ((uint32_t)buffer[4] << 8) | ((uint32_t)buffer[5]);
            sprintf_s(str, sizeof(str), "S/N : %lu", sn);
            serial_number->set_text(str);
            glutPostRedisplay();
        }
        
    }
    
}

DWORD WINAPI thread(LPVOID lpParam)
{
    TCHAR portName[32];
    uint16_t packet_length;

    while (true)
    {
        if (find_CDC_COMport_by_VID(_T("VID_0209"), portName, sizeof(portName)))
        {
            if (open_serial_port(portName))
            {     
                connection_status->set_text("Connection Status : Connected !");
                Sleep(100);
                packet_length = cmd_packet_make(CMD_GET_SN, g_tx_buffer, sizeof(g_tx_buffer));
                write_to_serial(g_tx_buffer, packet_length);
                break;
            }
        }
       
        Sleep(1000);
    }

    while (true) 
    {
        read_from_serial();    
    }

    CloseHandle(hSerial);
    return 0;
}

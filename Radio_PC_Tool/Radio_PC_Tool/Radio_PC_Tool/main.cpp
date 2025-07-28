#include <GL/glut.h>
#include <GL/glui.h>"
#include "serial.h"
#include "protocol.h"
#include <windows.h>
#include "resource.h"

int main_window = 0;
HANDLE h_thread;
DWORD thread_Id;
int g_hour = 0;
int g_min = 0;
GLfloat g_freq = 0.0f;
int g_delte_reservation_num = 0;
char g_input_freq[16] = ""; 
char g_input_num[16] = "";
GLUI_StaticText* connection_status;
GLUI_StaticText* serial_number;
uint8_t g_tx_buffer[64] = { 0, };

void callbackfunc(int control)
{
    uint16_t packet_length;
    switch (control)
    {
        case 2:
            g_freq = atof(g_input_freq);
            if (g_freq < 80.0 || g_freq > 110.0)
            {
                MessageBox(NULL, _T("유효하지 않은 주파수입니다. 다시 입력해주세요."), _T("라디오 예약 프로그램"), MB_OK);
                return;
            }
            //printf("%u,%u,%f\n", g_hour, g_min, g_freq);
            packet_length = data_packet_make((uint8_t)g_hour, (uint8_t)g_min, g_freq, g_tx_buffer, sizeof(g_tx_buffer));
            write_to_serial(g_tx_buffer, packet_length);
            MessageBox(NULL, _T("예약되었습니다."), _T("라디오 예약 프로그램"), MB_OK);
            break;
        case 4:
            g_delte_reservation_num = atoi(g_input_num);
            if (g_delte_reservation_num < 1 && g_delte_reservation_num > 6)
            {
                MessageBox(NULL, _T("유효하지 않은 예약번호입니다. 다시 입력해주세요."), _T("라디오 예약 프로그램"), MB_OK);
                return;
            }
            packet_length = cmd_packet_make(CMD_DELETE_RESERVATION, g_tx_buffer, sizeof(g_tx_buffer), g_delte_reservation_num);
            write_to_serial(g_tx_buffer, packet_length);
            MessageBox(NULL, _T("삭제되었습니다."), _T("라디오 예약 프로그램"), MB_OK);
        default:
            break;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc = 1;
    char* argv[1] = { (char*)"Tool" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(100, 100);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Radio Tool");  
    glutInitWindowPosition(900, 100);
    glutInitWindowSize(1000, 400);
    GLUI* main = GLUI_Master.create_glui("라디오 예약 프로그램", 0);
    main_window = main->get_glut_window_id();
    main->set_main_gfx_window(main_window);
    glutHideWindow();   

    HWND hwnd = FindWindow(NULL, TEXT("라디오 예약 프로그램")); 
    if (hwnd)
    {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
    }

    connection_status = main->add_statictext("");
    connection_status->set_text("Connection Status : Please connect your device");

    serial_number = main->add_statictext("");
    serial_number->set_text("S/N : ");
    new GLUI_StaticText(main, "");

    GLUI_Panel* reservation_panel = new GLUI_Panel(main, "Reservation");

    GLUI_Spinner* hour_spinner = main->add_spinner_to_panel(reservation_panel,"Hour :", GLUI_SPINNER_INT,&g_hour);
    hour_spinner->set_int_limits(0, 25, GLUI_LIMIT_WRAP);
    hour_spinner->set_speed(0.05);

    main->add_column_to_panel(reservation_panel, 0);

    GLUI_Spinner* min_spinner = main->add_spinner_to_panel(reservation_panel, "Min :", GLUI_SPINNER_INT, &g_min);

    min_spinner->set_int_limits(0, 60, GLUI_LIMIT_WRAP);
    min_spinner->set_speed(0.05);

    main->add_column_to_panel(reservation_panel, 0);

    GLUI_EditText* freq = main->add_edittext_to_panel(reservation_panel,"Radio freq(MHz) :", GLUI_EDITTEXT_TEXT, g_input_freq, 1, callbackfunc);
    freq->set_w(175);

    main->add_button("Add", 2, callbackfunc);

    new GLUI_StaticText(main, "");
    GLUI_Panel* delete_reservation_panel = new GLUI_Panel(main, "Delete Reservation", 0);
    GLUI_EditText* reservation_num = main->add_edittext_to_panel(delete_reservation_panel, "Num of reservation : ", GLUI_EDITTEXT_TEXT, g_input_num, 3, callbackfunc);
    delete_reservation_panel->set_w(400);

    main->add_button("Delete", 4, callbackfunc);

    h_thread = CreateThread(NULL, 0, thread, NULL, 0, &thread_Id);
    
    glutMainLoop();
    return 0;
}
#pragma once
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <GL/glut.h>
#include <GL/glui.h>"
#include "protocol.h"

extern uint8_t g_tx_buffer[64];
extern  GLUI_StaticText* connection_status;
extern GLUI_StaticText* serial_number;

DWORD WINAPI thread(LPVOID lpParam);
void write_to_serial(uint8_t* data, size_t length);

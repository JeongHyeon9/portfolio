#pragma once
#include <iostream>

#define PACKET_HEADER 0xAB
#define PACKET_START 0x01
#define PACKET_END 0x02
#define PACKET_TAIL 0xCD

#define PACKET_CMD 0x01
#define PACKET_DATA 0x00

#define CMD_GET_SN 0x11
#define CMD_DELETE_RESERVATION 0x12

uint16_t data_packet_make(uint8_t hour, uint8_t minute, float freq, uint8_t* outbuffer, uint16_t outbuffersize);
uint16_t cmd_packet_make(uint8_t cmd, uint8_t* outbuffer, uint16_t outbuffersize, uint8_t num_of_reservation = 1);
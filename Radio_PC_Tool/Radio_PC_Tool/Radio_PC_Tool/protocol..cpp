#include "protocol.h"

uint16_t  data_packet_make(uint8_t hour, uint8_t minute, float freq, uint8_t* outbuffer, uint16_t outbuffersize)
{
    uint16_t pos = 0;

    outbuffer[pos++] = PACKET_HEADER;
    outbuffer[pos++] = PACKET_START;
    outbuffer[pos++] = PACKET_DATA;
    outbuffer[pos++] = hour;
    outbuffer[pos++] = minute;

    memcpy(&outbuffer[pos], &freq, sizeof(float));
    pos += sizeof(float);

    outbuffer[pos++] = PACKET_END;
    outbuffer[pos++] = PACKET_TAIL;

    return pos;
}

uint16_t cmd_packet_make(uint8_t cmd, uint8_t* outbuffer, uint16_t outbuffersize, uint8_t num_of_reservation)
{
    uint16_t pos = 0;

    outbuffer[pos++] = PACKET_HEADER;
    outbuffer[pos++] = PACKET_START;
    outbuffer[pos++] = PACKET_CMD;
    outbuffer[pos++] = cmd;
    if (cmd == CMD_DELETE_RESERVATION)
    {
        outbuffer[pos++] = num_of_reservation;
    }
    outbuffer[pos++] = PACKET_END;
    outbuffer[pos++] = PACKET_TAIL;

    return pos;
}
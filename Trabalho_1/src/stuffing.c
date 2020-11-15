#include "stuffing.h"

int stuffing(char* packet, int packet_size, char* stuffed_bytes) {

    for (int i = 0; i < packet_size; i++) {
        if ((u_int8_t)*packet == 0x7e) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5e;
        }
        else if ((u_int8_t)*packet == 0x7d) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5d;
        }
        else {
            *stuffed_bytes = *packet;
        }
        stuffed_bytes++;
        packet++;
    }

    return 0;
}

int destuffing(char* packet, int packet_size, char* destuffed_bytes) {

    for (int i = 0; i < packet_size; i++) {
        if ((u_int8_t)*packet == 0x7d) {
            packet++;
            if ((u_int8_t)*packet == 0x5e) {
                *destuffed_bytes = 0x7e;
            }
            else if ((u_int8_t)*packet == 0x5d) {
                *destuffed_bytes = 0x7d;
            }
        }
        else {
            *destuffed_bytes = *packet;
            //printf("%X ", (u_int8_t)*iterator);
        }
        destuffed_bytes++;
        packet++;
    }

    return 0;
}

int numberFlagsToStuff(char* packet, int packet_size) {
    char* packet_iterator = packet;
    int new_bytes = 0;
    for (int i = 0; i < packet_size; i++) {
        if ((u_int8_t)*packet_iterator == 0x7e || (u_int8_t)*packet_iterator == 0x7d) {
            new_bytes++;
        }
        packet_iterator++;
    }
    return new_bytes;
}

int numberStuffedFlags(char* packet, int packet_size){
    char* packet_iterator = packet;

    int new_bytes = 0;
    for (int i = 0; i < packet_size; i++) {
        if ((u_int8_t)*packet_iterator == 0x7d) {
            new_bytes++;
        }
        packet_iterator++;
    }
    return new_bytes;
}

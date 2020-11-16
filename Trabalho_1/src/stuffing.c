#include "stuffing.h"

int stuffing(char* packet, int* packet_size, char* stuffed_bytes) {

    char bcc2 = 0;
    int current_size = *packet_size;

    char first_bytes[3];
    first_bytes[0] = stuffed_bytes[0];
    first_bytes[1] = stuffed_bytes[1]; 
    first_bytes[2] = stuffed_bytes[2]; 

    for (int i = 0; i < 3; i++) {
        if ((u_int8_t)first_bytes[i] == 0x7e) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5e;
            (*packet_size)++;
        }
        else if ((u_int8_t)first_bytes[i] == 0x7d) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5d;
            (*packet_size)++;
        }
        else {
            *stuffed_bytes = first_bytes[i];
        }
        stuffed_bytes++;
    }

    for (int i = 0; i < current_size - 3; i++) {
        bcc2 ^= *packet;
        if ((u_int8_t)*packet == 0x7e) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5e;
            (*packet_size)++;
        }
        else if ((u_int8_t)*packet == 0x7d) {
            *stuffed_bytes = 0x7d;
            stuffed_bytes++;
            *stuffed_bytes = 0x5d;
            (*packet_size)++;
        }
        else {
            *stuffed_bytes = *packet;
        }
        stuffed_bytes++;
        packet++;
    }

    (*packet_size)++;
    if (bcc2 == 0x7e) {
        *stuffed_bytes = 0x7d;
        stuffed_bytes++;
        *stuffed_bytes = 0x5e;
        (*packet_size)++;
    }
    else if (bcc2 == 0x7d) {
        *stuffed_bytes = 0x7d;
        stuffed_bytes++;
        *stuffed_bytes = 0x5d;
        (*packet_size)++;
    }
    else {
        *stuffed_bytes = bcc2;
    }

    return 0;
}

int destuffing(char* packet, int* packet_size, char* destuffed_bytes, char* final_packet) {
    int current_size = *packet_size;
    int packet_counter = 0;
    char bcc2 = 0;
    for (int i = 0; i < current_size; i++) {
        packet_counter++;
        if ((u_int8_t)*packet == 0x7d) {
            i++;
            packet++;
            if ((u_int8_t)*packet == 0x5e) {
                (*packet_size)--;
                *destuffed_bytes = 0x7e;
                if (i + 1 != current_size  && packet_counter > 3) {
                    bcc2 ^= *destuffed_bytes;
                    *final_packet = 0x7e;
                    final_packet++;
                }

            }
            else if ((u_int8_t)*packet == 0x5d) {
                (*packet_size)--;
                *destuffed_bytes = 0x7d;
                if (i + 1 != current_size && packet_counter > 3) {
                    bcc2 ^= *destuffed_bytes;
                    *final_packet = 0x7d;
                    final_packet++;
                }
            }
        }
        else {
            *destuffed_bytes = *packet;
            if (i + 1 != current_size && packet_counter > 3) {
                bcc2 ^= *destuffed_bytes;
                *final_packet = *packet;
                final_packet++;
            }
        }
        destuffed_bytes++;
        packet++;
    }

    return (uint8_t)bcc2;
}

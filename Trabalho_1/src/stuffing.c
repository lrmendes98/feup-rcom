#include "stuffing.h"

char* stuffing(char* packet, int* packet_size) {

    char* packet_iterator = packet;
    int new_bytes = 0;
    for (int i = 0; i < *packet_size; i++) {
        if ((uint8_t)*packet_iterator == 0x7e) {
            new_bytes++;
        }
        else if ((uint8_t)*packet_iterator == 0x7d) {
            new_bytes++;
        } 
        packet_iterator++;
    }

    *packet_size += new_bytes;
    char* stuffed_bytes = malloc(sizeof(char) * (*packet_size));
    char* iterator = stuffed_bytes;
    packet_iterator = packet;

    for (int i = 0; i < *packet_size - new_bytes; i++) {
        if ((uint8_t)*packet_iterator == 0x7e) {
            *iterator = 0x7d;
            iterator++;
            *iterator = 0x5e;
        }
        else if ((uint8_t)*packet_iterator == 0x7d) {
            *iterator = 0x7d;
            iterator++;
            *iterator = 0x5d;
        }
        else {
            *iterator = *packet_iterator;
            //printf("%X ", (uint8_t)*iterator);
        }
        iterator++;
        packet_iterator++;
    }
    //free(packet);

    return stuffed_bytes;
}

char* destuffing(char* packet, int* packet_size) {
    char* packet_iterator = packet;
    int new_bytes = 0;
    for (int i = 0; i < *packet_size; i++) {
        if ((uint8_t)*packet_iterator == 0x7e) {
            new_bytes++;
        }
        else if ((uint8_t)*packet_iterator == 0x7d) {
            new_bytes++;
        } 
        packet_iterator++;
    }

    *packet_size -= new_bytes;
    char* destuffed_bytes = malloc(sizeof(char) * (*packet_size));
    char* iterator = destuffed_bytes;
    packet_iterator = packet;
    
    for (int i = 0; i < *packet_size; i++) {
        if ((uint8_t)*packet_iterator == 0x7d) {
            packet_iterator++;
            if ((uint8_t)*packet_iterator == 0x5e) {
                *iterator = 0x7e;
            }
            else if ((uint8_t)*packet_iterator == 0x5d) {
                *iterator = 0x7d;
            }
        }
        else {
            *iterator = *packet_iterator;
        }
        
        iterator++;
        packet_iterator++;
    }
    //free(packet);

    return destuffed_bytes;
}

#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h> 

int stuffing(char* packet, int* packet_size, char* stuffed_bytes);

int destuffing(char* packet, int* packet_size, char* destuffed_bytes, char* final_packet);  

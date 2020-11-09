#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h> 

char* stuffing(char* packet, int* packet_size);

char* destuffing(char* packet, int* packet_size);   
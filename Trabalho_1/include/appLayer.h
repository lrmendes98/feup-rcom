#pragma once

#include "linkLayer.h"
#include "auxiliar.h"
#include "variables.h"

struct FileInfo {
    char * name;
    int size;
    int size_per_packet;
};

extern struct FileInfo file_info;

int appLayerWrite(int fd);

int appLayerRead(int fd);

int getFileInfo(char filename[]);

char* buildControlPacket(int type, int * packet_size);

char* buildDataPacket(FILE* filePtr, char* filename, int packet_nr,  int * packet_size);

char* readPacket(char* packet, char* file_ptr);

char* readStartPacket(char* packet, char* file_ptr);

char* readDataPacket(char* packet, char* file_ptr);

char* readEndPacket(char* packet, char* file_ptr);

int getFileSize(char* number_start);

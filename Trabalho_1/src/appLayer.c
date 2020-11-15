#include "appLayer.h"


struct FileInfo file_info;

int appLayerWrite(int fd)
{
    char file_name[50] = FILE_NAME;

    getFileInfo(file_name);

    int packet_size;
    char * initial_packet = buildControlPacket(2, &packet_size);
    if (llwrite(fd, initial_packet, packet_size) == -1)
        return -1;
    free(initial_packet);


    FILE *filePtr;
    filePtr = fopen(file_name,"rb");
    FILE *filePtr_copy = filePtr;
	if (!filePtr) {
		printf("Unable to open file!\n");
        return 1;
    }

    int packet_nr = 1;
    while(packet_size > 0) {
        char * packet = buildDataPacket(filePtr_copy, file_name, packet_nr, &packet_size); 
        if (packet_size == 0)
            break;
        packet_nr++;

        if (llwrite(fd, packet, packet_size) == -1)
            return -1;
        free(packet);
    }

    char * final_packet = buildControlPacket(3, &packet_size);
    if (llwrite(fd, final_packet, packet_size) == -1)
        return -1;
    free(final_packet);

    fclose(filePtr);

    printSuccess("All frames sent!\n");
    
    return 0;
}

int appLayerRead(int fd)
{
    
    char packet[PACKET_SIZE + 4];
    *packet = 0;
    char* file_ptr;
    
    while(*packet != 3) {
        // Read bytes
        if (llread(fd, packet) == -1)
            return -1;
        
        file_ptr = readPacket(packet, file_ptr);
    }

    printSuccess("All frames received!\n");
    printSuccess("File created!\n");

    return 0;
}

int getFileInfo(char filename[]){

    file_info.name = filename;

    struct stat file_stat;
    stat(filename, &file_stat);
    file_info.size = file_stat.st_size;

    file_info.size_per_packet = PACKET_SIZE;

    /*file_info.size_per_packet = file_info.size / 255;
    if (file_info.size % 255 > 0) 
        file_info.size_per_packet++;
    if (file_info.size_per_packet < 124) 
        file_info.size_per_packet = 124;*/

    return 0;
}

char* buildControlPacket(int type, int* packet_size) {
    int size = 0;
    char* packet;
    char * file = strrchr(file_info.name, '/');
    int file_name_size = (strlen(file_info.name) - 1) - ((long)file - (long)file_info.name);
    file++;
    packet = (char *) malloc(sizeof(char) * (9 + file_name_size));
    char *c = packet;

    size++;
    *c = type;
    c++;

    size++;
    *c = 0x00;
    c++;

    size++;
    *c = 0x04;
    c++;

    size += 4;
    *c = file_info.size/16777216;
    c++;
    *c = (file_info.size % 16777216) / 65536;
    c++;
    *c = (file_info.size % 65536) / 256;
    c++;
    *c = file_info.size % 256;
    c++;

    size++;
    *c = 0x01;
    c++;

    size++;
    *c = file_name_size;
    c++;

    for (int i = 0; i < file_name_size; i++) {
        size++;
        *c = *file;
        c++;
        file++;
    }

    *packet_size = size;

    return packet;
}

char* buildDataPacket(FILE* filePtr, char* filename, int packet_nr,  int * packet_size) {
    int size = 0;
    char* packet;
    int nr_bytes;

    if (file_info.size_per_packet * packet_nr <= file_info.size) {
        nr_bytes = file_info.size_per_packet;
    }
    else if (file_info.size - (file_info.size_per_packet * (packet_nr - 1)) > 0) {
        nr_bytes = file_info.size - (file_info.size_per_packet * (packet_nr - 1));
    }
    else {
        *packet_size = size;
        return (char*)-1;
    }

    packet = (char *) malloc(sizeof(char) * (nr_bytes + 4));
    char *c = packet;
    
    size++;
    *c = 0x01;
    c++;

    size++;
    *c = packet_nr;
    c++;

    size++;
    *c = nr_bytes / 256;
    c++;

    size++;
    *c = nr_bytes % 256;
    c++;
    
    size+= nr_bytes;
    fread(c, nr_bytes, 1, filePtr);

    *packet_size = size;

    return packet;
}

char* readPacket(char* packet, char* file_ptr) {
    u_int8_t packet_type = *packet;
    if(packet_type == 0x01) {
        file_ptr = readDataPacket(packet, file_ptr);
    }
    else if(packet_type == 0x02) {
        file_ptr = readStartPacket(packet, file_ptr);
    }
    else if(packet_type == 0x03) {
        file_ptr = readEndPacket(packet, file_ptr);
    }
    return file_ptr;
}

char* readStartPacket(char* packet, char* file_ptr){
    int size = getFileSize(packet + 3);
    file_ptr = malloc(sizeof(char) * size);
    return file_ptr;
}

char* readDataPacket(char* packet, char* file_ptr) {
    int data_size = 0;
    packet += 2;
    data_size += (u_int8_t)*packet * 256;
    packet++;
    data_size += (u_int8_t)*packet;
    packet++;

    memcpy(file_ptr, packet, data_size);
    file_ptr += data_size;
    return file_ptr;
}

char* readEndPacket(char* packet, char* file_ptr) {
    int size = getFileSize(packet + 3);
    packet += 8;
    int name_size = (u_int8_t)*packet;
    char folder[] = "copiedFiles/";
    char name[name_size + 13];
    strcpy(name, folder);
    packet++;

    for (int i = 0; i < name_size; i++) {
        name[i + 12] = *packet;
        packet++;
    }
    name[name_size + 12] = '\0';

    file_ptr -= size;

    FILE *file;
    file  = fopen (name, "w");
    fwrite(file_ptr, sizeof(char), size, file);
    fclose(file);

    return file_ptr;
}

int getFileSize(char* number_start) {
    int size = 0;
    size += (u_int8_t)*number_start * 16777216;
    number_start++;
    size += (u_int8_t)*number_start * 65536;
    number_start++;
    size += (u_int8_t)*number_start * 256;
    number_start++;
    size += (u_int8_t)*number_start;
    return size;
}

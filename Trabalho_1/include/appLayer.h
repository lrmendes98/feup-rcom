#pragma once

#include "linkLayer.h"
#include "auxiliar.h"

extern int packetSize;


int appLayerWrite(int fd);

int appLayerRead(int fd);

#ifndef ZWNJZWS_H
#define ZWNJZWS_H

#include <stdio.h>
struct textInfos{
    unsigned char * txtBuff;
    unsigned txtSize;
    char * path;
    FILE * txtPtr;
    unsigned size;
};

int initZw(char * txtPath, int encodeMode, char * text);
int encodeZw(char * text);
int decodeZw();
int fillTxtBuffer(const char * txt);
void addToFile(char bina);

#endif // ZWNJZWS_H

#ifndef LSB_H
#define LSB_H


#include <stdio.h>
#include <png.h>

enum Action{
    DECODE, ENCODE
};
/*
struct img{
    FILE * imgPtr;
    png_structp pngPtrR;
    png_structp pngPtrW;
    png_infop infoPng;
    png_bytepp rows;
    unsigned char header;
    png_uint_32 height;
    unsigned char * txtBuff;
    unsigned txtSize;
};*/

typedef struct png_t {
    int x, y;

    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;
    unsigned char * txtBuff;
} png_t;

int setInfos(char* imgPath);

int writeData(char * text);

int fileSize();

int fillTxtBufferLsb(char * txt);

void readTxtBuff();

int fillImgBuffer();

int initLsb(char * imgPath, unsigned encodeMode, char * text);

int encodeLsb(char * text, char * imgPath);

int decodeLsb();

int closeAll();

#endif // LSB_H

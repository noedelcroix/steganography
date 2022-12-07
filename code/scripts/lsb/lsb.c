#include "lsb.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "png.h"

struct png_t img;

int initLsb(char * imgPath, unsigned encodeMode, char * text){
    setInfos(imgPath);

    if(encodeMode){
        encodeLsb(text, imgPath);
        printf("Encoding finished\n");
    }else{
        decodeLsb();
        printf("Decoding finished\n");
    }

    for (img.y=0; img.y < img.height; img.y++) free(img.row_pointers[img.y]);
    free(img.row_pointers);
    return 0;
}

int setInfos(char * imgPath){
    char header[8];
    FILE *fp = fopen(imgPath, "rb");

    if(!fp){
        printf("Unable to open image. Check if the file exists and that it was correctly specified in the command.\n");
        return 1;
    }

    fread(header, 1, 8, fp);
    if(png_sig_cmp((unsigned char*)header, 0, 8)){
        printf("The file is not PNG file.\n");
        return 1;
    }

    img.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    img.info_ptr = png_create_info_struct(img.png_ptr);

    png_init_io(img.png_ptr, fp);
    png_set_sig_bytes(img.png_ptr, 8);

    png_read_info(img.png_ptr, img.info_ptr);

    img.width = png_get_image_width(img.png_ptr, img.info_ptr);
    img.height = png_get_image_height(img.png_ptr, img.info_ptr);
    img.color_type = png_get_color_type(img.png_ptr, img.info_ptr);
    img.bit_depth = png_get_bit_depth(img.png_ptr, img.info_ptr);
    img.number_of_passes = png_set_interlace_handling(img.png_ptr);

    png_read_update_info(img.png_ptr, img.info_ptr);

    img.row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * img.height);
    for (img.y=0; img.y < img.height; img.y++)
        img.row_pointers[img.y] = (png_byte*) malloc(png_get_rowbytes(img.png_ptr, img.info_ptr));

    png_read_image(img.png_ptr, img.row_pointers);
    fclose(fp);
    return 0;
}

int encodeLsb(char * text, char * imgPath){
    FILE *fp = fopen(imgPath, "wb");
    img.txtBuff=(unsigned char *)malloc(sizeof(unsigned char)*strlen(text)*8);
    if(!fp){
        printf("Unable to open image. Check if the file exists and that it was correctly specified in the command.\n");
        return 1;
    }

    img.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    img.info_ptr = png_create_info_struct(img.png_ptr);

    png_init_io(img.png_ptr, fp);

    png_set_IHDR(img.png_ptr, img.info_ptr, img.width, img.height, img.bit_depth, img.color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(img.png_ptr, img.info_ptr);
    writeData(text);
    png_write_image(img.png_ptr, img.row_pointers);
    png_write_end(img.png_ptr, NULL);

    return 0;
}

int fillTxtBufferLsb(char * txt){
    for(unsigned i=0; i<strlen(txt); i++){
        char currentChar=txt[i];
        for(int j=7; j>=0; j--){
            img.txtBuff[i*8+j]=currentChar%2;
            currentChar/=2;
        }
    }

    return 0;
}

int writeData(char * text){
    unsigned size=img.height*img.width*3;
    if (png_get_color_type(img.png_ptr, img.info_ptr) != PNG_COLOR_TYPE_RGB){
        printf("The color type is not supported.\n");
        return 1;
    }

    if(strlen(text)*8>size){
        printf("Text is too long for this file.\n");
        return 1;
    }
    fillTxtBufferLsb(text);

    unsigned counter=0;

    for(int i=0; i<img.height && counter<strlen(text)*8; i++){
        for(int j=0; j<img.width && counter<strlen(text)*8; j++){
            img.row_pointers[i][j]=(img.row_pointers[i][j]&0xFE)+img.txtBuff[counter];
            //printf("%d", img.row_pointers[i][j]);
            counter++;
        }
    }

    return 0;
}

int decodeLsb(){
    if (png_get_color_type(img.png_ptr, img.info_ptr) != PNG_COLOR_TYPE_RGB){
        printf("The color type is not supported.\n");
        return 1;
    }

    unsigned char currentChar=0;
    unsigned counter=0;
    for(int i=0; i<img.height; i++){
        for(int j=0; j<img.width; j++){
            if(counter%8==0 && counter!=0){
                //printf("%d %d\n", i, j);
                if(currentChar<127)
                printf("%c", currentChar);
                currentChar=0;
            }
            currentChar=(currentChar<<1) | (img.row_pointers[i][j]&0x01);
            counter++;
        }
    }

    return 0;
}

/*
int initLsb(char imgPath[], unsigned encodeMode, char * text){
    printf("here");
    img.txtBuff=NULL;
    img.imgPtr=fopen(imgPath, "r+b");

    if(!img.imgPtr){
        printf("Unable to open image. Check if the file exists and that it was correctly specified in the command.");
        return 1;
    }

    img.pngPtrR = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    img.infoPng = png_create_info_struct(img.pngPtrR);
    //png_read_info(img.pngPtrR, img.infoPng);
    img.height=png_get_image_height(img.pngPtrR, img.infoPng);

    png_init_io(img.pngPtrR, img.imgPtr);

    img.rows = (png_bytep*)malloc(sizeof(png_bytep) * img.height);
      for(unsigned y = 0; y < img.height; y++) {
        img.rows[y] = (png_byte*)malloc(png_get_rowbytes(img.pngPtrR,img.infoPng));
      }

    png_read_rows(img.pngPtrR, img.rows, NULL, img.height);

    if(encodeMode){
        encodeLsb(text);
    }else{
        decodeLsb();
    }



    return 0;
}
/*
int decodeLsb(){
    int rowbytes=png_get_rowbytes(img.pngPtrR, img.infoPng);
    img.txtBuff=(unsigned char *)malloc(sizeof(unsigned char)*img.height*rowbytes);
    for(unsigned i=0; i<img.height; i++){
        for(int j=0; j<rowbytes; j++){
            img.txtBuff[(i*rowbytes)+j] = img.rows[i][j]&0x01;
            if((i*rowbytes)+j<64)
            printf("%d ", img.rows[i][j]&0x01);
        }
    }

    //for(int i=0; i<65; i++) printf("%d", img.txtBuff[i]);

    for(unsigned i=0;i<img.height*rowbytes; i++){
        unsigned char currentChar=0;
        for(unsigned j=0; j<8; j++){
            currentChar=currentChar<<1|img.txtBuff[i]*(int)pow(2, j);
        }

        //printf("%d ", currentChar);

        //printf("%c", currentChar);
    }
    printf("\n");

    free(img.txtBuff);

    return 0;
}

int encodeLsb(char text[]){
    img.txtSize=strlen(text)*8;
    png_bytep * row_pointers=(png_bytep*)malloc(sizeof(png_bytep) * img.height);

    for(unsigned y = 0; y < img.height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(img.pngPtrR,img.infoPng));
      }
    png_read_image(img.pngPtrR, row_pointers);

    img.txtBuff=(unsigned char *)malloc(sizeof(unsigned char)*img.txtSize);
    fillTxtBufferLsb(text);
/*
    for(unsigned i=0; i<img.txtSize; i++){
        unsigned char * parsedPixel = &rows[(i/rowbytes)][i%rowbytes];
        *parsedPixel&=0xFE;
        *parsedPixel+=img.txtBuff[i];
        printf("%d", *parsedPixel);
    }*/
/*
    for(unsigned i=0; i<img.height; i++){
        png_bytep row;

        row=rows[i];

        for(int j=0; j<rowbytes; j++){
            png_byte pixel;
            pixel=row[j];
            row[j]=(pixel&0xFE)+img.txtBuff[(i*rowbytes)+j];
            printf("%d ", (i*rowbytes)+j);
            img.txtBuff[(i*rowbytes)+j]=pixel & 0x01;
        }
    }
    img.pngPtrW = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_init_io(img.pngPtrW, img.imgPtr);

    //png_set_IHDR(img.pngPtrR, img.infoPng, img.width, img.height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(img.pngPtrW, img.infoPng);
    png_write_image(img.pngPtrW, row_pointers);
    png_write_end(img.pngPtrW, NULL);

    for(unsigned y = 0; y < img.height; y++) {
        free(row_pointers[y]);
      }
      free(row_pointers);

      fclose(img.imgPtr);

      png_destroy_read_struct(&img.pngPtrR, &img.infoPng, NULL);
      png_destroy_write_struct(&img.pngPtrW, &img.infoPng);
      return 0;
}

int fillTxtBufferLsb(const char * txt){
    img.txtBuff = (unsigned char *) malloc(sizeof(unsigned char) * strlen(txt)*8);
    for(unsigned i=0; i<strlen(txt); i++){
        char currentChar=txt[i];
        for(int j=7; j>=0; j--){
            img.txtBuff[(i*8)+j]=currentChar%2;
            currentChar/=2;
        }
    }

    img.txtSize=strlen(txt)*8;

    return 0;
}*/

#include "zwnj-zws.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

wchar_t zwnj=0x200C;
wchar_t zws=0x200B;

struct textInfos textinfos;
int initZw(char txtPath[], int encodeMode, char text[]){
    textinfos.path=txtPath;
    setlocale(LC_CTYPE, "");
    if(encodeMode){
        encodeZw(text);
    }else{
        decodeZw();
    }
    return 0;
}

int fillTxtBuffer(const char * txt){
    textinfos.txtBuff = (unsigned char *) malloc(sizeof(unsigned char) * strlen(txt)*8);
    for(unsigned i=0; i<strlen(txt); i++){
        char currentChar=txt[i];
        for(int j=7; j>=0; j--){
            textinfos.txtBuff[(i*8)+j]=currentChar%2;
            currentChar/=2;
        }
    }

    textinfos.size=strlen(txt)*8;

    return 0;
}

int encodeZw(char * text){
    fillTxtBuffer(text);
    textinfos.txtPtr=fopen(textinfos.path, "rw+");
    if(textinfos.txtPtr==NULL){
        printf("Unable to open file. Check if the file exists and that it was correctly specified in the command.");
        return 1;
    }

    fseek(textinfos.txtPtr, -1, SEEK_END);

    for(unsigned i=0; i<textinfos.size; i++){
        addToFile(textinfos.txtBuff[i]);
    }

    fclose(textinfos.txtPtr);
    free(textinfos.txtBuff);
    return 0;
}

void addToFile(char bin){
    if(bin==1){
        fputwc(zwnj, textinfos.txtPtr);
    }else{
        fputwc(zws, textinfos.txtPtr);
    }
}

unsigned convertZw(const wchar_t val){
    return val==zwnj;
}

int decodeZw(){
    textinfos.txtPtr=fopen(textinfos.path, "r");
    wchar_t val;
    val=fgetwc(textinfos.txtPtr);
    while(val!=zwnj && val!=zws && !feof(textinfos.txtPtr)){
        val=fgetwc(textinfos.txtPtr);
    }

    unsigned counter=7;
    char currentChar=pow(2, counter)*convertZw(val);
    while(!feof(textinfos.txtPtr) && (val==zwnj || val==zws)){
        counter--;
        val=fgetwc(textinfos.txtPtr);
        currentChar+=pow(2, counter)*convertZw(val);
        if(counter==0){
            printf("%c", currentChar);
            counter=8;
            currentChar=0;
        }
    }

    printf("\n");

    fclose(textinfos.txtPtr);
    return 0;
}

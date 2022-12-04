//#include "scripts/lsb/lsb.h"
#include "scripts/zwnj-zws/zwnj-zws.h"
#include <string.h>

int main(int argc, char * argv[]){
    (void)argc;
    int encodeMode=strcmp(argv[2], "encode")==0;
    char * text="";
    if(encodeMode){
        text=argv[4];
    }

    if(strcmp(argv[1], "zero-width")==0){
        initZw(argv[3], encodeMode, text);
}else{
        printf("Commande incorrecte.");
    }

}

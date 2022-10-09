#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //Open the file and create a pointer to it
    FILE * ptr;
    ptr = fopen(argv[1], "rw");
    int fileSize;
    
    if (ptr == NULL) {
        return 1;
    }

//Get the file size
    fseek(ptr, 0, SEEK_END);
    fileSize = ftell(ptr);
    rewind(ptr);

//Read the file into a buffer
    char * buffer = (char *) malloc(sizeof(char) * fileSize);
    fread(buffer, 1, fileSize, ptr);

//Print the last bit of each byte
    for (int i = 0; i < fileSize; i++) {
        printf("%d", buffer[i] & 1);
    }
    printf("");

//TO DO LSB

//Close the file
    fclose(ptr);
    
    
    return 0;
}
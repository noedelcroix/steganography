demo : build
	@clear
	@echo "\033[1;31m##### Example zero-width #####\033[0m"
	@echo "\033[1;92mCreating example.txt...\033[0m"
	@echo "Hello World">example.txt
	@echo "\033[1;31mExample.txt content :\033[0m"
	@cat example.txt
	@echo "\033[1;31mExample.txt hex content :\033[0m"
	@hexdump example.txt
	@echo "\033[1;92mEncoding \"Bonjour\" in the example.txt file...\033[0m"
	./steganography zero-width encode $(shell pwd)/example.txt Bonjour
	@echo "\033[1;31mThe example.txt content looks not changed :\033[0m"
	@cat example.txt && echo
	@echo "\033[1;31mBut, the hex content change. There are additional characters :\033[0m"
	@hexdump example.txt
	@echo "\033[1;31mDecoding hidden data from the file :\033[0m"
	./steganography zero-width decode $(shell pwd)/example.txt

build : main.o zwnj-zws.o lsb.o
	gcc -g -Wall -o steganography main.o zwnj-zws.o lsb.o -lm -lpng -lz

clean : cleanObj
	rm -f steganography

cleanObj :
	rm -f *.o

main.o : main.c scripts/zwnj-zws/zwnj-zws.h scripts/lsb/lsb.h
	gcc -c main.c

zwnj-zws.o : scripts/zwnj-zws/zwnj-zws.c scripts/zwnj-zws/zwnj-zws.h
	gcc -c scripts/zwnj-zws/zwnj-zws.c
	
lsb.o : scripts/lsb/lsb.c scripts/lsb/lsb.h
	gcc -c scripts/lsb/lsb.c

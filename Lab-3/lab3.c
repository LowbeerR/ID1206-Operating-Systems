#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pageNumberMask = 0xff00;
int offsetMask = 0x00FF;
int frameNumber;
int physadr;
int pageFault = 0;
int total = 0;
int TLBHit = 0;
bool hit;

signed char *allo;

//TLB STRUCURE (queue / map ish),
struct scuffedMAP {
    int index;
    int hash[16];
    int toPhys[16];
};

int main(int argc, char *text[]) {
    if(argc != 2){
        return 1;
    }

    allo = malloc(65536);
    FILE *file = fopen(text[1], "r");
    int t[1000];
    int tmp = 0;
    char temp[100];
    while(fgets( temp, 100, file) != NULL){
        t[tmp++] = atoi(temp);
    }

    int pageTable[256];

    //create bogus page table, if init to -1 = bad number
    for (int i = 0; i < 256; i++)
        pageTable[i] = i;

    struct scuffedMAP scuffedMap;
    scuffedMap.index = 0;
    for (int i = 0; i < 16; i++) {
        scuffedMap.hash[i] = -1;
        scuffedMap.toPhys[i] = -1;
    }

    //page number: 255  Offset number: 254
    for (int i = 0; i < 20; i++) {
        int maskedInitVal = t[i] & 0xffff;
        hit = false;
        physadr = 0x0000;
        int page = (maskedInitVal & pageNumberMask) >> 8;
        int offset = maskedInitVal & offsetMask;


        //Look in the TLB for hits
        for (int j = 0; j < 16; j++) {
            if (scuffedMap.hash[j] == page && !hit) {
                physadr = scuffedMap.toPhys[j] << 8;
                physadr = physadr | offset;
                hit = true;
            }
        }
        //If not in TLB look in pageTable and update TLB
        if (!hit) {
            //Look in pageTable
            frameNumber = pageTable[page]; // blir 5
            physadr = physadr | (frameNumber << 8);
            physadr = physadr | offset;
            //Update TLB, queue is made with modulus calculations.
            scuffedMap.hash[scuffedMap.index % 16] = page;
            scuffedMap.toPhys[scuffedMap.index++ % 16] = frameNumber;
        }
        printf("Logical address: %d, Page: %d, Offset: %d, Physical address: %d, Value: %d\n", maskedInitVal, page, offset, physadr,*(allo + physadr));
    }

    //printf("Hello, World!%d\n", t[2]);
    return 0;
}

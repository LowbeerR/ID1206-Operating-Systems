#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pageNumberMask = 0xff00;
int offsetMask = 0x00FF;
int frameNumber;
int physadr;
int pageFault = 0;
float TLBHit = 0.0;
bool hit;

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
        pageTable[i] = -1;
    //pageTable[i] = i*256;

    struct scuffedMAP scuffedMap;
    scuffedMap.index = 0;
    for (int i = 0; i < 16; i++) {
        scuffedMap.hash[i] = -1;
        scuffedMap.toPhys[i] = -1;
    }
    //page number: 255  Offset number: 254
    for (int i = 0; i < 1000; i++) {
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
                TLBHit++;
            }
        }
        //If not in TLB look in pageTable and update TLB
        if (!hit) {
            //Look in pageTable
            if(pageTable[page] == -1){
                pageFault++;
                pageTable[page] = page*256;
            }
            frameNumber = pageTable[page]; // blir 5
            physadr = frameNumber | offset;

            //Update TLB, queue is made with modulus calculations.
            scuffedMap.hash[scuffedMap.index % 16] = page;
            scuffedMap.toPhys[scuffedMap.index++ % 16] = frameNumber;
        }

        FILE *bs;
        char read[256];
        bs = fopen("BACKING_STORE.bin", "r");
        if (bs == NULL) {
            perror("Error opening BACKING_STORE.bin");
            return 1;
        }
        fseek(bs, physadr, SEEK_SET);
        fread(read, 1, 256, bs);
        fclose(bs);
        int r = *(read);
        printf("Logical address: %d, Physical address: %d, Value: %d\n",
               maskedInitVal, physadr, r);
    }
    float hitrate = (TLBHit/ 1000.0);
    printf("TBL hit rate: %f, Page faults: %d\n", hitrate, pageFault);
    return 0;
}


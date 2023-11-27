#include <stdio.h>
#include <stdbool.h>

int pageNumberMask = 0xff00;
int offsetMask = 0x00FF;
int frameNumber;
int physadr;
int pageFault = 0;
bool hit;

//TLB STRUCURE (queue / map ish),
struct scuffedMAP {
    int index;
    int hash[16];
    int toPhys[16];
};

int main(void) { // 100000000
    int t[] = {1, 256, 32768, 32769, 128, 65534, 33153};
    int pageTable[256];

    //create bogus page table, if init to -1 = bad number
    for (int i = 0; i < 256; i++)
        pageTable[i] = i;

    struct scuffedMAP scuffedMap;
    scuffedMap.index = 0;
    for (int i = 0; i < 16; i++) {
    scuffedMap.hash[i] = -1;
    scuffedMap.toPhys[i] = -1;
    printf("%d,", scuffedMap.hash[i]);
    }
    printf("\n");

    //page number: 255  Offset number: 254
    for (int i = 0; i < 7; i++) {
        hit = false;
        physadr = 0x0000;
        int page = (t[i] & pageNumberMask) >> 8;
        int offset = t[i] & offsetMask;


        //Look in the TLB for hits
        for (int j = 0; j < 16; j++) {
            if (scuffedMap.hash[j] == page) {
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


        printf("Page: %d, Offset: %d, Physical address: %d\n", page, offset, physadr);
    }




    //printf("Hello, World!%d\n", t[2]);
    return 0;
}

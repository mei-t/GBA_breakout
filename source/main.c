// #include "gba_video.h"

int main(void){
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03; // not understand
    ioreg[1] = 0x04; // not understand

    // Write pixel colors into VRAM
    volatile unsigned short *vram = (unsigned short *)0x06000000;
    vram[80*240 + 115] = 0x001F; // X = 115, Y = 80, C = 000000000011111 = R
    vram[80*240 + 120] = 0x03E0; // X = 120, Y = 80, C = 000001111100000 = G
    vram[80*240 + 125] = 0x7C00; // X = 125, Y = 80, C = 111110000000000 = B

    // Wait forever
    while(1);

    return 0;
}
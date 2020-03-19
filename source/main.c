// #include "gba_video.h"

/* 16-bit volatile register */
#define VR16(x) *((volatile unsigned short *)(x))

#define REG_IOBASE 0x04000000
/* Vertical sync */
#define REG_VCOUNT VR16(REG_IOBASE + 0x6)

/* Size in pixels of screen in mode 3 */
#define MODE3_WIDTH 240
#define MODE3_HEIGHT 160

/* Wait for vertical sync */
void wait_vsync() {
    while (REG_VCOUNT >= MODE3_HEIGHT);
    while (REG_VCOUNT < MODE3_HEIGHT);
}

int main(void){
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03; // not understand
    ioreg[1] = 0x04; // not understand

    // Write pixel colors into VRAM
    volatile unsigned short *vram = (unsigned short *)0x06000000;
    // vram[80*240 + 115] = 0x001F; // X = 115, Y = 80, C = 000000000011111 = R
    // vram[80*240 + 120] = 0x03E0; // X = 120, Y = 80, C = 000001111100000 = G
    // vram[80*240 + 125] = 0x7C00; // X = 125, Y = 80, C = 111110000000000 = B

    int x = 0;

    for(int i=0; i<2; i++){
        for(int j=0; j<3; j++){
            vram[(2+i)*240 + j] = 0x7C00;
        }
    }
    vram[80*240] = 0xFFFF;
    // for(int i=0; i<10; i++){
    //     for(int j=0; j<3; j++){
    //         for(int k=0; k<2; k++){
    //             vram[2*240 + i]
    //         }
    //     }
    // }

    // while(1);

    // Wait forever
    while(1){
        // volatile char bottuns = ioreg[0x130];

        // // If Right pressed
        // if((bottuns & 0x4) == 0 && y < 240){
        //     y += 1;
        // }

        // // If Left pressed
        // if((bottuns & 0x5) == 0 && y > 0){
        //     y -= 1;
        // }
        // vram[80*240 + y] = 0x7C00;


        // unsigned short color = 0x001F;
        char buttons = ioreg[0x130];
        vram[80*240 + x] = 0x0;

        // if A is pressed
        if ((buttons & 0x1) == 0 && x < 239) {
            x += 1;
        }

        // if B is pressed
        if ((buttons & 0x2) == 0 && x > 0) {
            x -= 1;
        }

        // if left is pressed
        //if ((buttons & ))


        vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}

#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// BCD arithmetic support
#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

#define PLAYER_TILE 0x01 
#define PLAYER_PALETTE 0
unsigned char pad;
unsigned char x_pos = 128;
unsigned char y_pos = 120;
unsigned char speed = 1;
unsigned char playervis = 1;

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x03,			// screen color

  0x11,0x30,0x27,0x0,	// background palette 0
  0x1c,0x20,0x2c,0x0,	// background palette 1
  0x00,0x10,0x20,0x0,	// background palette 2
  0x06,0x16,0x26,0x0,   // background palette 3

  0x16,0x35,0x24,0x0,	// sprite palette 0
  0x00,0x37,0x25,0x0,	// sprite palette 1
  0x0d,0x2d,0x3a,0x0,	// sprite palette 2
  0x0d,0x27,0x2a	// sprite palette 3
};

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
}

void main(void)
{
  setup_graphics();
  // draw message  
  vram_adr(NTADR_A(2,2));
  vram_write("HELLO, WORLD!", 12);
  // enable rendering
  ppu_on_all();
  // infinite loop
  while(1) {
    ppu_wait_nmi();
    pad = pad_poll(0);
    oam_clear();
    if (playervis == 1) {
    oam_spr(x_pos, y_pos, PLAYER_TILE, PLAYER_PALETTE, 0);
    
    }
    if (pad & PAD_UP) {
      y_pos -= 1;
    }
    if (pad & PAD_DOWN) {
      y_pos += 1;
    }
    if (pad & PAD_LEFT) {
      x_pos -= 1;
    }
    if (pad & PAD_RIGHT) {
      x_pos += 1;
    }
  }
}

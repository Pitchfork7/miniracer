
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
unsigned char hasdash = 1;
unsigned char dashspeed = 30;
unsigned char dashtimelength = 2;
unsigned char dashtimer = 2 * 60;

// 1: down, 2: Up, 3: left, 4: right
unsigned char lastdir = 0;

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x2E,			// screen color

  0x07,0x00,0x27,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x00,0x16,0x00,0x00,	// sprite palette 0
  0x00,0x37,0x25,0x00,	// sprite palette 1
  0x0D,0x2D,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
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
    if (hasdash == 0) {
      dashtimer -= 1;
      if (dashtimer < 1) {
        dashtimer = dashtimelength * 60;
        hasdash = 1;
      }
    }
    if (playervis == 1) {
    oam_spr(x_pos, y_pos, PLAYER_TILE, PLAYER_PALETTE, 0);
    
    }
    if (pad & PAD_UP) {
      y_pos -= 1;
      lastdir = 2;
    }
    if (pad & PAD_DOWN) {
      y_pos += 1;
      lastdir = 1;
    }
    if (pad & PAD_LEFT) {
      x_pos -= 1;
      lastdir = 3;
    }
    if (pad & PAD_RIGHT) {
      x_pos += 1;
      lastdir = 4;
    }
    if (pad & PAD_B) {
      if (hasdash == 1) {
        switch (lastdir) {
          case 0: // nothing
            break;
          case 1: // down
            y_pos += dashspeed;
            hasdash = 0;
            break;
          case 2: // up
            y_pos -= dashspeed;
            hasdash = 0;
            break;
          case 3: // left
            x_pos -= dashspeed;
            hasdash = 0;
            break;
          case 4: //right
            x_pos += dashspeed;
            hasdash = 0;
            break;
            
            
        }
      }
    }
  }
}

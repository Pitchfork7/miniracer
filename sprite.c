
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
#define MENU_TILE 0x1f

#define PLAYER_TILE 0x01 
#define PLAYER_PALETTE 0

#define PALETTE1 0
unsigned char pad;

//menu vars

unsigned char menusel = 3;





//boss vars

unsigned char boss = 0;
unsigned char bossname[] = "";





// player vars
unsigned char x_pos = 128;
unsigned char y_pos = 120;
unsigned char speed = 1;
unsigned char playervis = 1;
unsigned char hasdash = 1;
unsigned char dashspeed = 30;
unsigned char dashtimelength = 2;
unsigned char dashtimer = 2 * 60;
// 1: menu, 2: game
unsigned char gamestate = 1;

// 1: down, 2: Up, 3: left, 4: right
unsigned char lastdir = 0;

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x2E,			// screen color

  0x07,0x00,0x27,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x07,0x16,0x26,0x00,   // background palette 3

  0x07,0x16,0x16,0x00,	// sprite palette 0
  0x00,0x37,0x25,0x00,	// sprite palette 1
  0x0D,0x2D,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
};

void clearscreen() {
  ppu_off();      // Must turn PPU off to write large amounts of VRAM
  vram_adr(0x2000);
  vram_fill(0x00, 1024); // Fill with blank tile
  vram_adr(0x2400);      // Clear the second nametable too if using mirroring
  vram_fill(0x00, 1024);
  oam_clear();    // Hide all sprites
  ppu_on_all();   // Turn it back on
}

void menu() {
  vram_adr(NTADR_A(10,2));
  vram_write("EYGPT SANDS:", 12);
  vram_adr(NTADR_A(4,7));
  vram_write("MENU:", 5);
  vram_adr(NTADR_A(4,8));
  vram_fill(0x78, 10);
  vram_adr(NTADR_A(4,22));
  vram_fill(0x78, 10);
  vram_adr(NTADR_A(6,11));
  vram_write("PLAY", 4);
  vram_adr(NTADR_A(6,15));
  vram_write("SETTINGS", 8);
  vram_adr(NTADR_A(6,19));
  vram_write("SOUND", 5);
}

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
  menu();
  // enable rendering
  ppu_on_all();
  // infinite loop
  while(1) {
    ppu_wait_nmi();
    oam_clear();
    
    
    switch (gamestate) {
      case 1:
        pad = pad_trigger(0);
	if (pad & PAD_UP) {
          menusel += 1;
      	}
      	if (pad & PAD_DOWN) {
          menusel -= 1;
      	}
        if (menusel < 1) {
         menusel = 3; 
        }
        if (menusel > 3) {
         menusel = 1; 
        }
        
        switch (menusel) {
          case 3:
            oam_spr(37, 87, MENU_TILE, PLAYER_PALETTE, 0);
            if (pad & PAD_START) {
                gamestate = 2;
          	clearscreen();
      	    }
            break;
          case 2:
            oam_spr(37, 119, MENU_TILE, PLAYER_PALETTE, 0);
            if (pad & PAD_START) {
          	clearscreen();
      	    }
            break;
          case 1:
            oam_spr(37, 151, MENU_TILE, PLAYER_PALETTE, 0);
            if (pad & PAD_START) {
          	break;
      	    }
            break;
            
        }
       break;
        
      case 2:  
      pad = pad_poll(0);
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
}

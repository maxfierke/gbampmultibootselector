#include <nds.h>
#include <nds/arm9/console.h> //basic print funcionality
#include <stdio.h>
#include <fat.h>
#include <nds/bios.h>
#include <nds/arm9/video.h>
#include <string.h>
#include <nds/system.h>
#include <nds/jtypes.h>
#include <sys/stat.h>
#include <nds/memory.h>
#include <malloc.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "bootgba.h"
#include "top_bin.h"
#include "bottom_bin.h"
#include "BootStrap_bin.h"
#include "file.h"
FILE *romfile;
int choosingfile = 1;
int FileChoose()
{
	powerON(POWER_ALL);
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	// Set up the sub screen
	videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE);
	vramSetMainBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_BG, VRAM_C_SUB_BG , VRAM_D_LCD);
	SUB_BG0_CR = BG_MAP_BASE(31);
	// Set the colour of the font to White.
	BG_PALETTE_SUB[255] = RGB15(31,31,31);
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
	consoleClear();
	romfile=NULL;
	romfile==loadFile();
	if(romfile == NULL) {
		return 0; // we didn't get a file
	}
	else {
		return 1; // we got a file
	}
}


void hdw_int(void) {
	powerON(POWER_ALL);
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	// Set up the sub screen
	videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE);
	vramSetMainBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_BG, VRAM_C_SUB_BG , VRAM_D_LCD);
	SUB_BG0_CR = BG_MAP_BASE(31);
	// Set the colour of the font to White.
	BG_PALETTE_SUB[255] = RGB15(31,31,31);
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
}
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	swiDelay(0x200000);		//bigger delay reduces sound pop
	irqInit();
	irqEnable(IRQ_VBLANK);
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	// set the sub background up for text display (we could just print to one
	// of the main display text backgrounds just as easily
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE); //sub bg 0 will be used to print text
    // set the first bank as background memory and the third as sub background memory
    // B and D are not used (if you want a bitmap greater than 256x256 you will need more
    // memory so another vram bank must be used and mapped consecutivly
    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_LCD, VRAM_C_SUB_BG_0x06200000 , VRAM_D_LCD);
	BG3_CR = BG_BMP16_256x256;
	SUB_BG3_CR = BG_BMP16_256x256;
	BG3_XDX = 1 << 8;
	BG3_XDY = 0;
	BG3_YDX = 0;
	BG3_YDY = 1 << 8;
	SUB_BG3_XDX = 1 << 8;
	SUB_BG3_XDY = 0;
	SUB_BG3_YDX = 0;
	SUB_BG3_YDY = 1 << 8;
	dmaCopy(top_bin, BG_GFX, 256*256*2);
	dmaCopy(bottom_bin, BG_GFX_SUB, 256*256*2);
	fatInitDefault();
	// Wait for user to press a button
	while(1)
	{
		scanKeys();
		while(keysHeld() & KEY_A) {
			REG_EXEMEMCNT &= ~(0x8080);
 			FILE *handle = fopen("devlauncher.nds", "rb");
 			if(handle < 0) {
 				iprintf("\nLoader has failed!\n");
 			}
 			//u32 fileCluster = GetFileCluster();
			struct stat st;
			u32 cluster;
			stat("/devlauncher.nds", &st);
			cluster = st.st_ino;
			fclose(handle);
 			REG_EXEMEMCNT |= (0x8080);
 			REG_IME = IME_DISABLE;	// Disable interrupts
 			REG_EXEMEMCNT |= (0x8080);  // ARM7 has access to GBA cart
 			*((vu32*)0x027FFFFC) = cluster;  // Start cluster of NDS to load
 			*((vu32*)0x027FFE04) = (u32)0xE59FF018;  // ldr pc, 0x027FFE24
 			*((vu32*)0x027FFE24) = (u32)0x027FFE04;  // Set ARM9 Loop address
 			swiSoftReset();  // Reset
		}

		while(keysHeld() & KEY_B) {
 			REG_EXEMEMCNT &= ~(0x8080);
 			FILE *handle = fopen("DSOrganize.nds", "rb");
 			if(handle < 0) {
 				iprintf("\nLoader has failed!\n");
 			}
 			//u32 fileCluster = GetFileCluster();
			struct stat st;
			u32 cluster;
			stat("/DSOrganize.nds", &st);
			cluster = st.st_ino;
			fclose(handle);
 			REG_EXEMEMCNT |= (0x8080);
 			REG_IME = IME_DISABLE;	// Disable interrupts
 			REG_EXEMEMCNT |= (0x8080);  // ARM7 has access to GBA cart
 			*((vu32*)0x027FFFFC) = cluster;  // Start cluster of NDS to load
 			*((vu32*)0x027FFE04) = (u32)0xE59FF018;  // ldr pc, 0x027FFE24
 			*((vu32*)0x027FFE24) = (u32)0x027FFE04;  // Set ARM9 Loop address
 			swiSoftReset();  // Reset
		}

		while(keysHeld() & KEY_X) {
			REG_EXEMEMCNT &= ~(0x8080);
 			FILE *handle = fopen("moonshell.nds", "rb");
 			if(handle < 0) {
 				iprintf("\nLoader has failed!\n");
 			}
 			//u32 fileCluster = GetFileCluster();
			struct stat st;
			u32 cluster;
			stat("/moonshell.nds", &st);
			cluster = st.st_ino;
			fclose(handle);
 			REG_EXEMEMCNT |= (0x8080);
 			REG_IME = IME_DISABLE;	// Disable interrupts
 			REG_EXEMEMCNT |= (0x8080);  // ARM7 has access to GBA cart
 			*((vu32*)0x027FFFFC) = cluster;  // Start cluster of NDS to load
 			*((vu32*)0x027FFE04) = (u32)0xE59FF018;  // ldr pc, 0x027FFE24
 			*((vu32*)0x027FFE24) = (u32)0x027FFE04;  // Set ARM9 Loop address
 			swiSoftReset();  // Reset
		}
		while(keysHeld() & KEY_Y) {
			hdw_int();
			struct stat st;
			char filename[256];
			iprintf("\x1b[2J");
			// Wait two VBlanks as instructed in the FAT docs
			swiWaitForVBlank();
			swiWaitForVBlank();
			chdir("/");
			FileChoose();
			break;
		}
		while(keysHeld() & KEY_START) {

			REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;
			REG_IPC_FIFO_TX = 1;

			BootGBA();
		}
		swiWaitForVBlank();
	}
	return 0;
}



/* 
   TINY3D sample / (c) 2010 Hermes  <www.elotrolado.net>

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include <io/pad.h>

#include <tiny3d.h>
#include <libfont.h>

// font 0: 224 chr from 32 to 255, 16 x 32 pix 2 bit depth
#include "font.h"

// font 1: 224 chr from 32 to 255, 16 x 32 pix 2 bit depth
#include "font_b.h"

// font 2: 255 chr from 0 to 254, 8 x 8 pix 1 bit depth
extern unsigned char msx[];


// draw one background color in virtual 2D coordinates

void DrawBackground2D(u32 rgba)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(0  , 0  , 65535);
    tiny3d_VertexColor(rgba);

    tiny3d_VertexPos(847, 0  , 65535);

    tiny3d_VertexPos(847, 511, 65535);

    tiny3d_VertexPos(0  , 511, 65535);
    tiny3d_End();
}

void drawScene()
{
	float x, y;
    tiny3d_Project2D(); // change to 2D context (remember you it works with 848 x 512 as virtual coordinates)
    DrawBackground2D(0x0040ffff) ; // light blue 

    SetFontSize(12, 24);
    
    x= 0.0; y = 0.0;

    SetCurrentFont(1);
    SetFontColor(0xffffffff, 0x0);
	SetFontAutoCenter(0);    
    x = DrawString(x,y, "welcome to another crackme of ");
    SetFontColor(0x00ff00ff, 0x0);
    //SetCurrentFont(1);
    x = DrawString(x,y, "HACKvent ");
    //SetCurrentFont(1);
    SetFontColor(0xffffffff, 0x0);
    x = DrawString(x,y, "can you find the hidden flag?");

	
	int i;
	//char str[]   =  "HV17-5mJ3-yxcm-WiUX-nZgW-e0lT";	
	unsigned char str1[] 	= { 0x08,0x33,0xcf,0xa8,0xa0,0x3d,0x5e,0xac,0xa1,0x73,0x69,0xf4,0x57,0x37,0xaa,0xc2,0x26,0xee,0xfc,0x61,0xf8,0x79,0xa4,0xcb,0xe8,0x1d,0xb5,0x21,0xb6 };	
	unsigned char str2[] 	= { 0x2b,0xdb,0x0d,0xf9,0x06,0xe8,0x24,0xbe,0xc2,0x2a,0x6d,0xb5,0x12,0x63,0x04,0x9a,0x8e,0x84,0x14,0xf9,0x5f,0x56,0x3d,0x82,0x80,0xa6,0x6d,0x95,0xc6 };
	//unsigned char str3[] 	= { 0x6a,0xbe,0xf3,0x67,0x8b,0xe1,0x17,0x58,0x51,0x75,0x7d,0x38,0x27,0x39,0x83,0x0f,0xc1,0x3f,0xb0,0xb5,0xc8,0x74,0xff,0x1f,0x45,0xdf,0xe8,0xd8,0x24 };
	unsigned char str3[] 	= { 0x6a,0xbe,0xf3,0x67,0x8b,0x99,0x0f,0x63,0x5b,0x75,0x63,0x3a,0x32,0x01,0x83,0x2c,0x9b,0x28,0x8f,0xb5,0xe9,0x1d,0xf2,0x27,0x45,0xd3,0x9f,0xd9,0x1c };
	char output[30];	
	for (i=0; i<29; i++)
	{
		char c = str1[i] ^ str2[i];
		if (c & 1){
			c = c ^ 1;
		}
	    output[i] = c;
	}
	for (i=0; i<29; i++)
	{
		char c = output[i] ^ str3[i];		
	    output[i] = c;
	}
	output[i] = '\0';
	SetFontAutoCenter(1);
	x = DrawString(0,y+32, output);
    y += 64;
    SetCurrentFont(1);
    SetFontSize(16, 16);
    SetFontColor(0xffffffff, 0x000000ff);
    SetFontAutoCenter(1);
    DrawString(0, y, "HV17-Ju5t-s0me-fak3-FlaG-4y0u");
    SetFontAutoCenter(0);  

}


void LoadTexture()
{

    u32 * texture_mem = tiny3d_AllocTexture(64*1024*1024); // alloc 64MB of space for textures (this pointer can be global)    

    u32 * texture_pointer; // use to asign texture space without changes texture_mem

    if(!texture_mem) return; // fail!

    texture_pointer = texture_mem;

    ResetFont();
    texture_pointer = (u32 *) AddFontFromBitmapArray((u8 *) font  , (u8 *) texture_pointer, 32, 255, 16, 32, 2, BIT0_FIRST_PIXEL);
    texture_pointer = (u32 *) AddFontFromBitmapArray((u8 *) font_b, (u8 *) texture_pointer, 32, 255, 16, 32, 2, BIT0_FIRST_PIXEL);
    texture_pointer = (u32 *) AddFontFromBitmapArray((u8 *) msx   , (u8 *) texture_pointer,  0, 254,  8,  8, 1, BIT7_FIRST_PIXEL);

    // here you can add more textures using 'texture_pointer'. It is returned aligned to 16 bytes
}



s32 main(s32 argc, const char* argv[])
{
	padInfo padinfo;
	padData paddata;
	int i;
	
	tiny3d_Init(1024*1024);

	ioPadInit(7);

	// Load texture

    LoadTexture();

	
	// Ok, everything is setup. Now for the main loop.
	while(1) {

        /* DRAWING STARTS HERE */

        // clear the screen, buffer Z and initializes environment to 2D

        tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);

        // Enable alpha Test
        tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

        // Enable alpha blending.
        tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
            TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
            TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
      

		// Check the pads.
		ioPadGetInfo(&padinfo);

		for(i = 0; i < MAX_PADS; i++){

			if(padinfo.status[i]){
				ioPadGetData(i, &paddata);
				
				if(paddata.BTN_CROSS){
					return 0;
				}
			}
			
		}

        drawScene(); // Draw

        /* DRAWING FINISH HERE */

        tiny3d_Flip();
		
	}

	return 0;
}


![screenshot](demo2.jpg)

# P2501
P2501 is an emulator of Video Shifter, the most simple type of video controller.  
The goal of this project is to create a simple video interface to use in the creation of more complex emulators (ex. CHIP8).  
The emulator use modern OpenGl as render and rely on Glfw and Glew.  
This repository contains all the dependencies and all that you need to build the video device

# FUNCTIONS

### GlVideo *GlVideoInit(int width, int height)  
Initialize the video device and return a pointer to it.  
Width and Height are the current screen resolution in pixels.

### void GlVideoClose(GlVideo **video)
Close the video device and free all the internal allocated resources.  

### void GlVideolAttachMemory(GlVideo *video, byte *mem)
Attach an external memory to the video device.

### void GlVideoEnable(GlVideo *video)
Enable the video device.

### void GlVideoDisable(GlVideo *video)
Disable the video device.

### int GlVideoState(GlVideo *video)
Return 1 if the device is enabled, otherwise 0.

### void GlVideoUpdate(GlVideo *video)
Transfer the linked memory to the internal memory of the device.

### void GlVideoRender(GlVideo *video)
Render to the display the content of the video memory.

### void GlVideoSetColor(GlVideo *video, byte r, byte g, byte b)
Set the current color for the "ON" pixels of the video device.  

# EXAMPLE 1

Display an external memory of 256 bytes on a video device with resolution 64x32 setting the "ON" pixel to blue.  
The first byte of the memory is set to 0xAA (10101010).

``` 
#include "../GLVIDEO/glvideo.h"

/**********/
int main (int argc, char **argv)
{
    byte *mem = Calloc(64*32/8, sizeof(byte));

    mem[0] = 0xAA;

    GlVideo *video = GlVideoInit(64,32);

    GlVideoAttachMemory(video, mem);

    GlVideoEnable(video);

    GlVideoUpdate(video);

    GlVideoSetColor(video, 0x00, 0x00, 0xFF);

    while(GlVideoState(video)==1)
    {
        GlVideoRender(video);
    }

    GlVideoClose(&video);

    Free(mem);

    return 0;
}
``` 
Output :  
![screenshot](demo1.jpg)

# EXAMPLE 2

Initialize an external memory with a raw ".pbm" file and display it on a video device setting the "ON" pixel to green.

``` 
#include "../GLVIDEO/glvideo.h"
#include "pbm.h"

/**********/
int main (int argc, char **argv)
{
    int w,h = 0;

    byte *mem = NULL;
    
    PbmLoad("test.pbm", &mem, &w, &h);

    GlVideo *video = GlVideoInit(w,h);

    GlVideoAttachMemory(video, mem);

    GlVideoEnable(video);

    GlVideoUpdate(video);

    GlVideoSetColor(video, 0x00, 0xFF, 0x00);

    while(GlVideoState(video)==1)
    {
        GlVideoRender(video);
    }

    GlVideoClose(&video);

    Free(mem);

    return 0;
}

``` 
Output :  
![screenshot](demo2.jpg)

# EXAMPLE 3

Using a Mazectric rule generate in memory a simple maze and display it on a video device setting the "ON" pixel to yellow.

``` 
#include "../GLVIDEO/glvideo.h"
#include "string.h"
#include "stdlib.h"

void SetBit(byte **mem, int w, int h, int row, int col, int val)
{
    int num_pixel = w*row + col;

    int num_byte = num_pixel / BYTE_SIZE;

    int num_bit = num_pixel % BYTE_SIZE;

    if(val==1) { (byte)(*mem)[num_byte] |= (0x80 >> num_bit); }
    else { (byte)(*mem)[num_byte] &= ~(0x80 >> num_bit); }
}

//
int GetBit(byte *mem, int w, int h, int row, int col)
{
    int num_pixel = w*row + col;

    int num_byte = num_pixel / BYTE_SIZE;

    int num_bit = num_pixel % BYTE_SIZE;
    
    int val = (mem[num_byte]>>(BYTE_SIZE-1-num_bit));

	if(val%2 == 1){ return 1; }
	
    return 0;
}

//
int Maze(byte *cur, byte **next, int cols, int rows)
{
    int res = 0;

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            int adj = 0;

            adj += ((i-1>=0) && (i-1< rows) && (j-1>=0) && (j-1< cols) && ( GetBit(cur, cols, rows, i-1, j-1) == 1));
            adj += ((i-1>=0) && (i-1< rows) && (j  >=0) && (j  < cols) && ( GetBit(cur, cols, rows, i-1, j  ) == 1));
            adj += ((i-1>=0) && (i-1< rows) && (j+1>=0) && (j+1< cols) && ( GetBit(cur, cols, rows, i-1, j+1) == 1));
            adj += ((i  >=0) && (i  < rows) && (j-1>=0) && (j-1< cols) && ( GetBit(cur, cols, rows, i  , j-1) == 1));
            adj += ((i  >=0) && (i  < rows) && (j+1>=0) && (j+1< cols) && ( GetBit(cur, cols, rows, i  , j+1) == 1));
            adj += ((i+1>=0) && (i+1< rows) && (j-1>=0) && (j-1< cols) && ( GetBit(cur, cols, rows, i+1, j-1) == 1));
            adj += ((i+1>=0) && (i+1< rows) && (j  >=0) && (j  < cols) && ( GetBit(cur, cols, rows, i+1, j  ) == 1));
            adj += ((i+1>=0) && (i+1< rows) && (j+1>=0) && (j+1< cols) && ( GetBit(cur, cols, rows, i+1, j+1) == 1));

            if( GetBit(cur, cols, rows, i, j)==1 )
            {
                if(adj==1 || adj==2 || adj==3) { SetBit(next, cols, rows, i, j, 1); }
                else { SetBit(next, cols, rows, i, j, 0);}
            }
            else
            {  
                if(adj==2) { SetBit(next, cols, rows, i, j, 1);  res=1; }
                else { SetBit(next, cols, rows, i, j, 0);}
            } 
            
        }
    }

    return res;
}


/**********/
int main (int argc, char **argv)
{
    int w = 64;
    int h = 32;

    byte *core_mem = Calloc(w*h/BYTE_SIZE,sizeof(byte));
    byte *copy_mem = Calloc(w*h/BYTE_SIZE,sizeof(byte));

    SetBit(&core_mem,w,h,16,7,1);
    SetBit(&core_mem,w,h,16,8,1);
    SetBit(&core_mem,w,h,17,7,1);

    while( Maze(core_mem, &copy_mem, w, h)==1)
    {
        memcpy(core_mem, copy_mem, w*h/8);
    }
        
    GlVideo *video = GlVideoInit(w,h);

    GlVideoAttachMemory(video, copy_mem);

    GlVideoEnable(video);

    GlVideoUpdate(video);

    GlVideoSetColor(video, 0xFF, 0xFF, 0x00);

    while(GlVideoState(video)==1)
    {
        GlVideoRender(video);
    }

    GlVideoClose(&video);

    Free(core_mem);
    Free(copy_mem);

    return 0;
}

``` 
Output :  
![screenshot](demo3.jpg)

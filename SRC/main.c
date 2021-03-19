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
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

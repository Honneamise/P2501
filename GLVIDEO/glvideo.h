#ifndef GLVIDEO_H
#define GLVIDEO_H

//debug
#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRT_SECURE_NO_WARNINGS
#include "crtdbg.h"
#endif

//std
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

//opengl
#define GLFW_INCLUDE_NONE
#define GLFW_DLL
#include "../GLFW/INCLUDE/glfw3.h"
#include "../GLEW/INCLUDE/glew.h"

//core
#include "SRC/funcs.h"
#include "SRC/shader.h"
#include "SRC/ogl.h"

#define GlVideo                     OglVideo
#define GlVideoInit(w,h)            OglSetupVideo(w, h)
#define GlVideoClose(v)             OglCloseVideo(v)
#define GlVideoAttachMemory(v,m)    OglAttachMemory(v,m);
#define GlVideoEnable(v)            OglVideoEnable(v)
#define GlVideoDisable(v)           OglVideoDisable(v)
#define GlVideoState(v)             OglVideoState(v)
#define GlVideoUpdate(v)            OglUpdate(v);
#define GlVideoRender(v)            OglRender(v);
#define GlVideoSetColor(v,r,g,b)    OglSetPixelColor(v,r,g,b);

#endif
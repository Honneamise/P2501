#ifndef OGL_H
#define OGL_H

//defines
#define OGL_MAJOR 4
#define OGL_MINOR 5

#define VERT_SHADER_FILE "vert.glsl"
#define FRAG_SHADER_FILE "frag.glsl"

#define PIXEL_COLOR_LOC "PixelColor"

#define DEFAULT_WINDOW_WIDTH 800

//typedef
typedef struct OglVideo
{
    int enable;

    int pixels_w;
    int pixels_h;

    byte *link;
    int link_size;

    byte *mem;
    int mem_size;

    GLFWwindow *window;

    GLfloat vertices[16];

    GLuint vao;
    GLuint vbo;
    GLuint vert_shader;
    GLuint frag_shader;
    GLuint prog_shader;
    GLuint texture;

} OglVideo;

//glfw
void OglGlfwErrorCallback(int error, const char* description);

void OglGlfwWindowCloseCallback(GLFWwindow* window);

void OglGlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void OglGlfwFramebufferCallback(GLFWwindow* window, int width, int height);

void OglSetupGlfw(OglVideo *video);

//glew
void OglSetupGlew();

//opengl
char* OglDebugSource(GLenum source);

char* OglDebugType(GLenum type);

char* OglDebugSeverity(GLenum severity);

void GLAPIENTRY
OglDebugCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* param);

GLuint OglShader(GLenum type, char *src);

GLuint OglProgramShader(GLuint vert_shader, GLuint frag_shader);

void OglSetupGl(OglVideo *video);

//utils
void OglSetupVertices(OglVideo *video);

void OglUpdateTextureData(OglVideo *video);

//core
OglVideo *OglSetupVideo(int w, int h);

void OglCloseVideo(OglVideo **video);

void OglAttachMemory(OglVideo *video, byte *mem);

void OglVideoEnable(OglVideo *video);

void OglVideoDisable(OglVideo *video);

int OglVideoState(OglVideo *video);

void OglUpdate(OglVideo *video);

void OglRender(OglVideo *video);

void OglSetPixelColor(OglVideo *video, byte r, byte g, byte b);

#endif
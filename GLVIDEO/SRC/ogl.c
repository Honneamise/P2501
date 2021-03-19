#include "../glvideo.h"

/****************/
/* GLFW SECTION */
/****************/

/**********/
void OglGlfwErrorCallback(int error, const char* description)
{
    ExitError("[%s]: %s", __func__, description);
}

void OglGlfwWindowCloseCallback(GLFWwindow* window)
{
    OglVideo *video = glfwGetWindowUserPointer(window);

	video->enable = 0;
}

/**********/
void OglGlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{ 
		OglVideo *video = glfwGetWindowUserPointer(window);

		video->enable = 0;
	}
}

/**********/
void OglGlfwFramebufferCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

/**********/
void OglSetupGlfw(OglVideo *video)
{
	glfwSetErrorCallback(OglGlfwErrorCallback);

    if(!glfwInit()) { ExitError("[%s]: GLFW init failed", __func__); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);

	float ratio = (float)video->pixels_h/(float)video->pixels_w;

	video->window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, (int)( (float)DEFAULT_WINDOW_WIDTH*ratio), "", NULL, NULL);
    
    if(video->window==NULL)
    {
        glfwTerminate();
        ExitError("[%s]: GLFW window failed", __func__); 
    }

    glfwMakeContextCurrent(video->window);

    glfwSwapInterval(1);

	glfwSetWindowCloseCallback(video->window, OglGlfwWindowCloseCallback);

	glfwSetKeyCallback(video->window, OglGlfwKeyCallback);

	glfwSetFramebufferSizeCallback(video->window, OglGlfwFramebufferCallback);

	glfwSetWindowUserPointer(video->window, video); 
}

/****************/
/* GLEW SECTION */
/****************/

/**********/
void OglSetupGlew()
{
	if (glewInit() != GLEW_OK) { ExitError("[%s]: GLEW init failed", __func__); }
}

/******************/
/* OPENGL SECTION */
/******************/

/**********/
char *OglDebugSource(GLenum source)
{
    switch (source)
	{
        case GL_DEBUG_SOURCE_API: 				return "GL_DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: 	return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: 	return "GL_DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: 		return "GL_DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: 		return "GL_DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: 			return "GL_DEBUG_SOURCE_OTHER";
	}

	return "UNKNOW";
}

/*********/
char *OglDebugType(GLenum type)
{
	switch (type)
	{
        case GL_DEBUG_TYPE_ERROR: 				return "GL_DEBUG_TYPE_ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: 	return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: 		return "GL_DEBUG_TYPE_PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: 		return "GL_DEBUG_TYPE_PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: 				return "GL_DEBUG_TYPE_MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: 			return "GL_DEBUG_TYPE_PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: 			return "GL_DEBUG_TYPE_POP_GROUP";
        case GL_DEBUG_TYPE_OTHER: 				return "GL_DEBUG_TYPE_OTHER";
	}

	return "UNKNOW";
}

/**********/
char *OglDebugSeverity(GLenum severity)
{
	switch (severity)
	{
        case GL_DEBUG_SEVERITY_HIGH: 			return "GL_DEBUG_SEVERITY_HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM: 			return "GL_DEBUG_SEVERITY_MEDIUM";
        case GL_DEBUG_SEVERITY_LOW: 			return "GL_DEBUG_SEVERITY_LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION: 	return "GL_DEBUG_SEVERITY_NOTIFICATION";
	}

	return "UNKNOW";
}

/**********/
void GLAPIENTRY
OglDebugCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* param)
{
	if(severity!=GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		printf("[%s] Source : %s, Type : %s, Severity : %s, Message : %s\n",
				__func__,
				OglDebugSource(source),
				OglDebugType(type),
				OglDebugSeverity(severity),
				message
			);
	}
}

/**********/
GLuint OglShader(GLenum type, char *src)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const GLchar**)&src, NULL);

	glCompileShader(shader);

	GLint check = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &check);

	if (check == GL_FALSE)
	{
		int len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		char* msg = Calloc(len, sizeof(char));

		glGetShaderInfoLog(shader, len, &len, msg);

		ExitError("[%s]: Shader compile error : %s", __func__, msg);
	}

	return shader;
}

/**********/
GLuint OglProgramShader(GLuint vert_shader, GLuint frag_shader)
{
	GLuint prog_shader = glCreateProgram();

	glAttachShader(prog_shader, vert_shader);

	glAttachShader(prog_shader, frag_shader);

	glLinkProgram(prog_shader);

	GLint check = GL_FALSE;

	glGetProgramiv(prog_shader, GL_LINK_STATUS, &check);

	if (check == GL_FALSE)
	{
		int len = 0;

		glGetProgramiv(prog_shader, GL_INFO_LOG_LENGTH, &len);

		char* msg = Calloc(len, sizeof(char));

		glGetProgramInfoLog(prog_shader, len, &len, msg);

		ExitError("[%s]: Shader link error : %s", __func__, msg);
	}

	return prog_shader;
}

/**********/
void OglSetupGl(OglVideo *video)
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OglDebugCallback, NULL);

	glEnable(GL_MULTISAMPLE);

	float ratio = (float)video->pixels_h/(float)video->pixels_w;

    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, (int)( (float)DEFAULT_WINDOW_WIDTH*ratio) );

	glGenVertexArrays(1, &video->vao);
    glBindVertexArray(video->vao); 

	glGenBuffers(1, &video->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, video->vbo);

	glEnableVertexAttribArray(0);//vertex 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
    glEnableVertexAttribArray(1);//texture 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)) );

	OglSetupVertices(video);

	glBufferData(GL_ARRAY_BUFFER, sizeof(video->vertices), video->vertices, GL_STATIC_DRAW);	
	
	glGenTextures(1, &video->texture);
	glBindTexture(GL_TEXTURE_2D, video->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, video->pixels_w, video->pixels_h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    video->vert_shader = OglShader(GL_VERTEX_SHADER, VERT_SRC);

    video->frag_shader = OglShader(GL_FRAGMENT_SHADER, FRAG_SRC);

    video->prog_shader = OglProgramShader(video->vert_shader, video->frag_shader);

	glUseProgram(video->prog_shader);

}

/*****************/
/* UTILS SECTION */
/*****************/

/**********/
void OglSetupVertices(OglVideo *video)
{
	//vertex coords + uv

	video->vertices[0] = -1.0f; 
	video->vertices[1] = -1.0f;
	video->vertices[2] =  0.0f; 
	video->vertices[3] =  1.0f;

	video->vertices[4] = -1.0f;
	video->vertices[5] =  1.0f;
	video->vertices[6] =  0.0f; 
	video->vertices[7] =  0.0f;

	video->vertices[8]  =  1.0f;
	video->vertices[9]  = -1.0f;
	video->vertices[10] =  1.0f; 
	video->vertices[11] =  1.0f;

	video->vertices[12] = 1.0f,
	video->vertices[13] = 1.0f;
	video->vertices[14] = 1.0f; 
	video->vertices[15] = 0.0f; 
}

/**********/
void OglUpdateTextureData(OglVideo *video)
{
	if(video->link==NULL)
	{ 
		ExitError("[%s]: no memory attached\n");
	}
	else
	{
		int count = 0;

		for(int i=0;i<video->link_size;i++)
		{
			byte b = video->link[i];

			for(int j=BYTE_SIZE-1;j>=0;j--)
			{
				int val = (b>>j);

				if(val%2 == 1 ){ video->mem[count] = 0xFF; }
				else { video->mem[count] = 0x00; }
				count++;
			} 
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, video->pixels_w, video->pixels_h, GL_RED, GL_UNSIGNED_BYTE, video->mem);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

/****************/
/* CORE SECTION */
/****************/

/**********/
OglVideo *OglSetupVideo(int w, int h)
{
	if(w%BYTE_SIZE != 0 || h%BYTE_SIZE != 0)
	{ 
		ExitError("[%s]: invalid resolution format, sizes must be multiple of BYTE_SIZE\n",__func__);
	};

	OglVideo *video = Calloc(1, sizeof(OglVideo));

	video->pixels_w = w;
	video->pixels_h = h;

	video->link = NULL;
	video->link_size = w * h / BYTE_SIZE;

	video->mem = Calloc(w*h, sizeof(byte));
	video->mem_size = w*h;

	OglSetupGlfw(video);

	OglSetupGlew();

	OglSetupGl(video);	

	int loc = glGetUniformLocation(video->prog_shader, PIXEL_COLOR_LOC);
    glUniform3f(loc, 1.0f, 1.0f, 1.0f); //defaultpixel color white

	return video;
}

/**********/
void OglCloseVideo(OglVideo **video)
{
	glDeleteTextures(1, &(*video)->texture);

	glDetachShader( (*video)->prog_shader, (*video)->vert_shader);
	glDeleteShader( (*video)->vert_shader);

	glDetachShader( (*video)->prog_shader, (*video)->frag_shader);
	glDeleteShader( (*video)->frag_shader);

	glDeleteProgram( (*video)->prog_shader);

	glfwDestroyWindow( (*video)->window);
 
    glfwTerminate();

	Free( (*video)->mem);
	
	Free(*video);

	*video = NULL;
}

/**********/
void OglAttachMemory(OglVideo *video, byte *mem )
{
	if(mem==NULL) 
	{ 
		ExitError("[%s]: link is NULL\n");
	};

	video->link = mem;	
}

/**********/
void OglVideoEnable(OglVideo *video)
{
	if(video->link==NULL) { ExitError("[%s]: cant enable link is NULL\n",__func__); }

	video->enable = 1;
}

/**********/
void OglVideoDisable(OglVideo *video)
{
	video->enable = 0;
}

/**********/
int OglVideoState(OglVideo *video)
{
	return video->enable;
}

/**********/
void OglUpdate(OglVideo *video)
{
	if(video->enable==0) { ExitError("[%s]: video is not enabled\n",__func__); }
	if(video->link==NULL) { ExitError("[%s]: video link is NULL\n",__func__); }

	OglUpdateTextureData(video);
}

/**********/
void OglRender(OglVideo *video)
{
	if(video->enable==0) { ExitError("[%s]: video is not enabled\n",__func__); }
	if(video->link==NULL) { ExitError("[%s]: video link is NULL\n",__func__); }
	
	glClearColor(1.0f,1.0f,1.0f,1.0f);//fuori

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glfwSwapBuffers(video->window);

    glfwPollEvents();

}

/**********/
void OglSetPixelColor(OglVideo *video, byte r, byte g, byte b)
{
	int loc = glGetUniformLocation(video->prog_shader, PIXEL_COLOR_LOC);
    glUniform3f(loc, (float)r/0xFF, (float)g/0xFF, (float)b/0xFF );
}
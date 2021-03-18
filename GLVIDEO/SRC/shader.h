#ifndef SHADER_H
#define SHADER_H

#define VERT_SRC "\
#version 330\n\
\n\
layout (location = 0) in vec2 Pos;\n\
layout (location = 1) in vec2 Uv;\n\
\n\
out vec2 Tex;\n \
\n\
void main()\n\
{\n\
    gl_Position = vec4( Pos, 0.0f, 1.0f);\n\
\n\
    Tex = Uv;\n\
}\n"

#define FRAG_SRC "\
#version 330\n\
\n\
in vec2 Tex;\n\
\n\
uniform sampler2D Texture;\n\
\n\
uniform vec3 PixelColor;\n\
\n\
void main()\n\
{\n\
    float val = texture(Texture, Tex).r;\n\
\n\
    vec4 final = vec4(val*PixelColor, 1.0f);\n\
\n\
    gl_FragColor = final;\n\
}\n"

#endif
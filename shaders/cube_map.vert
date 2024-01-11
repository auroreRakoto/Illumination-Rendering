#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 View;
uniform mat4 Perspective;
//uniform mat4 Model;
out vec3 TexCoords;

void main()
{
    TexCoords = position;
    //gl_Position = Perspective * View * Model * vec4(position, 1.0);
    gl_Position = Perspective * View  *  vec4(position, 1.0);
}

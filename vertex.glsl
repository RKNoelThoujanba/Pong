#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


out vec2 texCords;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    //gl_Position = u * vec4(aPos.x, aPos.y, 0.0, 1.0);
    texCords = aTexCords;
} 

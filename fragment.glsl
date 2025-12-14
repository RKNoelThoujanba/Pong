#version 450 core

out vec4 finalColor;

in vec2 texCords;
uniform sampler2D u_TexSlot;

void main()
{
    finalColor = texture(u_TexSlot, texCords);
}

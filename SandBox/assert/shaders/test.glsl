#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;



out vec2 v_TexCoord;

void main()
{
    v_TexCoord  =  a_TexCoord;
    gl_Position =  u_ViewProjection * u_ModelMatrix * vec4(a_Position , 1.0);
    
}

#type fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
color = texture(u_Texture,v_TexCoord);
}
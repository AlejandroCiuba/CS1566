#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4x4 ctm;
uniform mat4x4 projection;
uniform mat4x4 model_view;

void main()
{
	texCoord = vTexCoord;
	gl_Position = projection * model_view * ctm * vPosition;
	color = vColor;
}
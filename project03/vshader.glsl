#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTexCoord;
varying vec2 texCoord;
varying vec4 color;

uniform mat4x4 ctm;
uniform mat4x4 mvm;
uniform mat4x4 perm;

uniform bool isTriangle;

void main()
{
	texCoord = vTexCoord;
	if(isTriangle)
		gl_Position = vPosition;
	else
		gl_Position = perm * mvm * ctm * vPosition;
	color = vColor;
}
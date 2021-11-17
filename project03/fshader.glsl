#version 120

varying vec2 texCoord;
varying vec4 color;
varying float count;

uniform sampler2D texture;
uniform bool use_color;

void main()
{	
	if(use_color)
		gl_FragColor = color;
	else
		gl_FragColor = texture2D(texture, texCoord);
}
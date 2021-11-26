#version 120

varying vec2 texCoord;
varying vec4 color;

uniform sampler2D texture;
uniform int use_texture;

void main()
{
	//gl_FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
	if(use_texture == 1) gl_FragColor = texture2D(texture, texCoord);
	else gl_FragColor = color;
}
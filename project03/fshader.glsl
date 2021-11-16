#version 120

varying vec2 texCoord;
varying vec4 color;
varying float count;

uniform sampler2D texture;

void main()
{			
	gl_FragColor = color;
	gl_FragColor = texture2D(texture, texCoord);
}
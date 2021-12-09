#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vColor;
varying vec4 color;

uniform mat4x4 ctm;
uniform mat4x4 projection;
uniform mat4x4 model_view;
uniform vec4 light_pos;
uniform float shininess;
vec4 ambient, diffuse, specular;

// control lighting affect
uniform int lighting;

void main() {

	if(lighting == 1) {

		gl_Position = projection * model_view * ctm * vPosition;
		
		// Calculate the ambient, diffuse, and specular
		// AMBIENT
		ambient = .2 * vColor;

		// Calculate the normal relative to the transformations applied here
		vec4 N = normalize(model_view * ctm * vNormal);

		// Light source in the object frame
		vec4 L_temp = model_view * (light_pos - (ctm * vPosition));
		vec4 L = normalize(L_temp);

		// DIFFUSE
		diffuse = max(dot(L, N), 0.0) * vColor;

		// SPECULAR
		vec4 eye = vec4(0.0,0.0,0.0,1.0); // According to model_view
		vec4 V = normalize(eye - model_view * ctm * vPosition);
		vec4 H = normalize(L + V);
		specular = pow(max(dot(N, H), 0.0), shininess) * vec4(.25,.25,.25,1);

		// Get the distance from the light source
		float distance = length(L_temp);

		// ATTENUATION
		float attenuation = 1;

		color = ambient + attenuation * (diffuse + specular);
	}
	else {
		gl_Position = projection * model_view * ctm * vPosition;
		color = vColor;
	}
}
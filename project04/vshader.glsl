#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vColor;
varying vec4 color;

uniform mat4x4 ctm;
uniform mat4x4 projection;
uniform mat4x4 model_view;
/*uniform vec4 light_pos;
uniform float shininess; // For specular
uniform float att_const, att_lin, att_quad;
uniform vec4 amb_prod, diff_prod, spec_prod;*/
vec4 ambient, diffuse, specular;

void main() {

	gl_Position = projection * model_view * ctm * vPosition;
	
	/*
	// Calculate the ambient, diffuse, and specular
	// AMBIENT
	ambient = amb_prod;

	// Calculate the normal relative to the transformations applied here
	vec4 N = normalize(model_view * ctm * vNormal);

	// Light source in the object frame
	vec4 L_temp = model_view * (light_pos - (ctm * vPosition));
	vec4 L = normalize(L_temp);

	// DIFFUSE
	diffuse = max(dot(L, N), 0.0) * diff_prod;

	// SPECULAR
	vec4 eye = vec4(0.0,0.0,0.0,1.0); // According to model_view
	vec4 V = normalize(eye - model_view * ctm * vPosition);
	vec4 H = normalize(L + V);
	specular = pow(max(dot(N, H), 0,0), shininess) * spec_prod;

	// Get the distance from the light source
	float distance = length(L_temp);

	// ATTENUATION
	attenuation = 1 / (att_const + (att_lin * distance) + (att_quad * distance * distance));
	*/

	color = vColor;//ambient + attenuation * (diffuse + specular);
}
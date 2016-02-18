#version 330 core

layout (location = 0) in vec4 iVelocity;
layout (location = 2) in ivec4 iConnections;

uniform int WIDTH;
uniform int HEIGHT;

uniform samplerBuffer tex_pressure;

out vec4 tf_velocity;

vec4 boundary() {
	return iVelocity;
}

vec4 inner() {
	float L = texelFetch(tex_pressure, iConnections[0]).y;
	float T = texelFetch(tex_pressure, iConnections[1]).y;
	float R = texelFetch(tex_pressure, iConnections[2]).y;
	float B = texelFetch(tex_pressure, iConnections[3]).y;

	return vec4(iVelocity.xy - 0.5 * vec2(R-L, T-B), 0, 1);
}

void main() {
	vec4 outVelocity;
	if(iConnections[0] == -1 || iConnections[1] == -1 || iConnections[2] == -1 || iConnections[3] == -1) {
		outVelocity = boundary();
	}
	else {
		outVelocity = inner();
	}

	tf_velocity = outVelocity;
}
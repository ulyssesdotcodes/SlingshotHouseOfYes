#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_pressure;

in float gLayer;
out vec2 fragColor;

void main() {
	vec2 outDP;
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution.xyz;

	float L = texture(tex_pressure, pos + vec3(1, 0, 0) / i_resolution.xyz).y;
	float R = texture(tex_pressure, pos + vec3(-1, 0, 0) / i_resolution.xyz).y;
	float T = texture(tex_pressure, pos + vec3(0, -1, 0) / i_resolution.xyz).y;
	float B = texture(tex_pressure, pos + vec3(0, 1, 0) / i_resolution.xyz).y;
	float U = texture(tex_pressure, pos + vec3(0, 0, 1) / i_resolution.xyz).y;
	float D = texture(tex_pressure, pos + vec3(0, 0, -1) / i_resolution.xyz).y;

	float bC = texture(tex_pressure, pos).x;

	fragColor = vec2(bC, (L + R + B + T + U + D - bC) / 6);
}

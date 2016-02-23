#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_pressure;
uniform sampler3D tex_velocity;

in float gLayer;
out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution.xyz;

	float L = texture(tex_pressure, pos + vec3(-1, 0, 0) / i_resolution.xyz).y;
	float R = texture(tex_pressure, pos + vec3(1, 0, 0) / i_resolution.xyz).y;
	float T = texture(tex_pressure, pos + vec3(0, 1, 0) / i_resolution.xyz).y;
	float B = texture(tex_pressure, pos + vec3(0, -1, 0) / i_resolution.xyz).y;
	float U = texture(tex_pressure, pos + vec3(0, 0, 1) / i_resolution.xyz).y;
	float D = texture(tex_pressure, pos + vec3(0, 0, -1) / i_resolution.xyz).y;

	vec4 velocity = texture(tex_velocity, pos);

	fragColor = vec4(velocity.xyz - 1.125 * vec3(R-L, T-B, U-D), 1);
}
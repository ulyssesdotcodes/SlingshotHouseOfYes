#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_velocity;

in float gLayer;
out vec2 fragColor;

void main() {
	vec4 outDP;
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution.xyz;

	vec4 T = texture(tex_velocity, pos + vec3(0, 1, 0) / i_resolution.xyz);
	vec4 B = texture(tex_velocity, pos + vec3(0, -1, 0) / i_resolution.xyz);
	vec4 U = texture(tex_velocity, pos + vec3(0, 0, 1) / i_resolution.xyz);
	vec4 D = texture(tex_velocity, pos + vec3(0, 0, -1) / i_resolution.xyz);
	vec4 L = texture(tex_velocity, pos + vec3(-1, 0, 0) / i_resolution.xyz);
	vec4 R = texture(tex_velocity, pos + vec3(1, 0, 0) / i_resolution.xyz);

	fragColor = vec2(((R.x - L.x) + (T.y - B.y) + (U.z - D.z)) * 0.5, 0);
}

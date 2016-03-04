#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_velocity;

out vec4 fragColor;

void main() {
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	vec4 L = texture2D(tex_velocity, pos + vec2(-1, 0) / i_resolution.xy);
	vec4 T = texture2D(tex_velocity, pos + vec2(0, 1) / i_resolution.xy);
	vec4 R = texture2D(tex_velocity, pos + vec2(1, 0) / i_resolution.xy);
	vec4 B = texture2D(tex_velocity, pos + vec2(0, -1) / i_resolution.xy);

	fragColor = vec4(((R.y - L.y) - (T.x - B.x)) * 0.5, 0, 0, 1);
}

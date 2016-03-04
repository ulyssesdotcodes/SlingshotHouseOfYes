#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_fire;

uniform float i_dt;
uniform float i_mult;

out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	vec3 f = texture2D(tex_fire, pos).xyz;

	fragColor = vec4(f * i_mult, 1);
}

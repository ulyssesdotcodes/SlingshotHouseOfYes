#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_vorticity;
uniform sampler2D tex_velocity;

uniform float i_dt;

float EPSILON = 0.00024414;

out vec4 fragColor;

void main() {
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	vec4 L = texture2D(tex_vorticity, pos + vec2(-1, 0) / i_resolution.xy);
	vec4 T = texture2D(tex_vorticity, pos + vec2(0, 1) / i_resolution.xy);
	vec4 R = texture2D(tex_vorticity, pos + vec2(1, 0) / i_resolution.xy);
	vec4 B = texture2D(tex_vorticity, pos + vec2(0, -1) / i_resolution.xy);

	vec4 C = texture2D(tex_vorticity, pos);

	vec2 force = 0.5 *	vec2(abs(T.x) - abs(B.x), abs(R.x) - abs(L.x));

	float magSqr = max(EPSILON, dot(force, force));
	force = force * sqrt(magSqr) * 1024;

	force *= C.xy * vec2(1, -1) * 0.0325;

	vec2 uNew = texture2D(tex_velocity, pos).xy;

	fragColor = vec4(uNew + i_dt * force, 0, 1);
}

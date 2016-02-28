#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_pressure;
//uniform sampler2D tex_divergence;
uniform sampler2D tex_obstacles;

out vec4 fragColor;

void main() {
	vec2 outDP;
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	float L = texture2D(tex_pressure, pos + vec2(-1, 0) / i_resolution.xy).y;
	float T = texture2D(tex_pressure, pos + vec2(0, 1) / i_resolution.xy).y;
	float R = texture2D(tex_pressure, pos + vec2(1, 0) / i_resolution.xy).y;
	float B = texture2D(tex_pressure, pos + vec2(0, -1) / i_resolution.xy).y;

	float pC = texture2D(tex_pressure, pos).y;

	vec4 oL = texture2D(tex_obstacles, pos + vec2(-1, 0) / i_resolution.xy);
	vec4 oT = texture2D(tex_obstacles, pos + vec2(0, 1) / i_resolution.xy);
	vec4 oR = texture2D(tex_obstacles, pos + vec2(1, 0) / i_resolution.xy);
	vec4 oB = texture2D(tex_obstacles, pos + vec2(0, -1) / i_resolution.xy);

	if(oL.x > 0) L = pC;
	if(oT.x > 0) T = pC;
	if(oR.x > 0) R = pC;
	if(oB.x > 0) B = pC;

	float bC = texture2D(tex_pressure, pos).x;
	fragColor = vec4(bC, (L + R + B + T - bC) * .16, 0, 1);
}

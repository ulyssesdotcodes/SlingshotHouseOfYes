#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_pressure;
uniform sampler2D tex_velocity;
uniform sampler2D tex_obstacles;

out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	float L = texture2D(tex_pressure, pos + vec2(-1, 0) / i_resolution.xy).y;
	float T = texture2D(tex_pressure, pos + vec2(0, 1) / i_resolution.xy).y;
	float R = texture2D(tex_pressure, pos + vec2(1, 0) / i_resolution.xy).y;
	float B = texture2D(tex_pressure, pos + vec2(0, -1) / i_resolution.xy).y;
	float C = texture2D(tex_pressure, pos ).y;

	vec4 oL = texture2D(tex_obstacles, pos + vec2(-1, 0) / i_resolution.xy);
	vec4 oT = texture2D(tex_obstacles, pos + vec2(0, 1) / i_resolution.xy);
	vec4 oR = texture2D(tex_obstacles, pos + vec2(1, 0) / i_resolution.xy);
	vec4 oB = texture2D(tex_obstacles, pos + vec2(0, -1) / i_resolution.xy);

	vec2 obsV = vec2(0);
	vec2 vMask = vec2(1);

	if(oL.x > 0) { L = C; obsV.x = oL.y; vMask.x = 0; } 
	if(oR.x > 0) { R = C; obsV.x = oR.y; vMask.x = 0; } 
	if(oT.x > 0) { T = C; obsV.y = oT.x; vMask.y = 0; } 
	if(oB.x > 0) { B = C; obsV.y = oB.x; vMask.y = 0; } 
	
	vec2 oldVel = texture2D(tex_velocity, pos).xy;
	vec2 vel = oldVel - 0.5 * vec2(R-L, T-B);
	fragColor = vec4(vMask * vel, 0, 1);
}

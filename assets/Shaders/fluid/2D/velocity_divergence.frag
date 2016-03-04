#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_velocity;
uniform sampler2D tex_obstacles;

out vec4 fragColor;

void main() {
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;

	vec4 L = texture2D(tex_velocity, pos + vec2(-1, 0) / i_resolution.xy);
	vec4 T = texture2D(tex_velocity, pos + vec2(0, 1) / i_resolution.xy);
	vec4 R = texture2D(tex_velocity, pos + vec2(1, 0) / i_resolution.xy);
	vec4 B = texture2D(tex_velocity, pos + vec2(0, -1) / i_resolution.xy);

	//vec4 oL = texture2D(tex_obstacles, pos + vec2(-1, 0) / i_resolution.xy);
	//vec4 oT = texture2D(tex_obstacles, pos + vec2(0, 1) / i_resolution.xy);
	//vec4 oR = texture2D(tex_obstacles, pos + vec2(1, 0) / i_resolution.xy);
	//vec4 oB = texture2D(tex_obstacles, pos + vec2(0, -1) / i_resolution.xy);

	//if(oL.x > 0) L = vec4(0);
	//if(oT.x > 0) T = vec4(0);
	//if(oR.x > 0) R = vec4(0);
	//if(oB.x > 0) B = vec4(0);

	fragColor = vec4((R.x - L.x) + (T.y - B.y) * 0.5, 0, 0, 1);
}

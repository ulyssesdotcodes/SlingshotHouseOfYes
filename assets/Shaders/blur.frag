#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_prev;

out vec4 fragColor;

void main() {
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;
	vec3 current = texture2D(tex_prev, pos).xyz;

	for(int i = 0; i < 7; i++) {
	  
	}

	//if(obstacles.x > 0) {
	//	fragColor = vec4(0);
	//	return;
	//}

	vec2 mSDP = vec2(0.1, 1.0 - 0.8);

	vec2 dropDistance = pos - mSDP;

	//float density = max(0, 0.008 - dot(dropDistance, dropDistance)) * i_dt * max(i_beat, 0.125) * 2.0;
	//float density = max(0, 0.004 - dot(dropDistance, dropDistance)) * i_dt * 1024;

	//density *= mix(0.6, 1.0, rand(vec2(pos.x * pos.y, cos(i_dt))));

	float density = i_dt * mix(0.0, 8.0, cnoise(vec3(i_time * 0.125, pos * 8)));

	float temp = current.y + i_dt * (pos.y) * 0.5;

	fragColor = vec4(current.x + density, temp, 0, 1);
}

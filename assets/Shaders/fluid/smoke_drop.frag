#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_prev;

uniform vec2 i_smokeDropPos;
uniform float i_volume;
uniform float i_beat;
uniform float i_dt;
uniform float i_time;

in float gLayer;
out vec4 fragColor;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution;
	vec3 current = texture(tex_prev, pos).xyz * 0.99;

	vec3 mSDP = vec3(i_smokeDropPos.x, 1.0 - i_smokeDropPos.y, 0);

	vec3 dropDistance = pos - mSDP;

	//float density = max(0, 0.008 - dot(dropDistance, dropDistance)) * i_dt * max(i_beat, 0.125) * 2.0;
	float density = max(0, 0.008 - dot(dropDistance, dropDistance)) * i_dt * 1024;

	density *= mix(0.6, 1.0, rand(vec2(pos.x * pos.y, cos(i_dt))));

	float temperature = current.y + density * 256;

	float hue = current.z;
	if(density > 0.0001) {
		hue = 0.22;
	}

	fragColor = vec4(current.x + density, temperature, hue, 1);
	//fragColor = vec4(dropDistance, 1);
}
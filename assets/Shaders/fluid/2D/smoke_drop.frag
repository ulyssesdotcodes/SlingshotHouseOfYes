#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_prev;
uniform sampler2D tex_hoy;

uniform vec2 i_smokeDropPos;
uniform float i_volume;
uniform float i_beat;
uniform float i_dt;

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
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;
	vec3 current = texture2D(tex_prev, pos).xyz * 0.98;

	vec3 hoy = texture2D(tex_hoy, pos).xyz;

	vec2 mSDP = vec2(i_smokeDropPos.x, 1.0 - i_smokeDropPos.y);

	vec2 dropDistance = pos - mSDP;

	//float density = max(0, 0.008 - dot(dropDistance, dropDistance)) * i_dt * 0.25;//* max(i_beat, 0.125) * pow(i_volume, 0.5);

	float density = hoy.x * 0.04;

	density *= mix(0, 1.0, rand(vec2(pos))) * (1.0 - abs(pos.x - 0.5) * 1.5);

	float temperature = current.y + density * 10000;

	float hue = current.z;
	if(density > 0.0001) {
		hue = 0.22;
	}

	fragColor = vec4(current.x + density, temperature, hue, 1);
}
#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_prev;
uniform sampler2D tex_obstacles;

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
	vec3 current = texture2D(tex_prev, pos).xyz;

	vec4 obstacles = texture2D(tex_obstacles, pos);

	if(obstacles.x > 0) {
		fragColor = vec4(0);
		return;
	}

	float density = 0.04 * i_dt * mix(0, 1.0, rand(vec2(pos)));

	float hue = current.z;

	fragColor = vec4(current.x + density, 0, hue, 1);
}
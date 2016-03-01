#version 330 core

uniform sampler2D tex_smoke;
uniform sampler2D tex_obstacles;

uniform vec2 i_resolution;

out vec4 fragColor;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 position = gl_FragCoord.xy / i_resolution.xy;
	vec4 smoke = texture2D(tex_smoke, position);

	vec4 obstacles = texture2D(tex_obstacles, position);

	if(obstacles.x > 0) {
		fragColor = vec4(0);
		return;
	}

	//vec3 cloudColor = vec3(0.52, 0.8, 0.91);
	//vec3 cloudColor = vec3(0.54, 0.43, 0.92);
	vec3 cloudColor = vec3(0.54, 0.2, 0.2);

	if(smoke.x > 0) {
		fragColor = vec4(hsv2rgb(mix(cloudColor, vec3(0.54, 0, 0), smoke.x)), 1);
		return;
	}

	fragColor = vec4(hsv2rgb(cloudColor), 1);
}

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

	if(smoke.x > 0) {
		fragColor = vec4(hsv2rgb(vec3(smoke.z, 0.6, smoke.x * 2)), 1);
		return;
	}

	fragColor = vec4(vec3(0), 1);
}

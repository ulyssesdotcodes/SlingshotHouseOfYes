#version 330 core

uniform vec2 i_resolution;
uniform vec2 i_target_resolution;
uniform sampler2D tex_velocity;
uniform sampler2D tex_target;
uniform sampler2D tex_obstacles;
uniform float i_dt;
uniform float i_dissipation;

out vec4 fragColor;

vec4 inner(vec2 targetPos) {
	vec2 velocity = texture2D(tex_velocity, targetPos).xy;
	vec2 resPos = floor(targetPos * i_target_resolution) + 0.5;
	vec2 tracedPos = resPos - i_dt * velocity * i_target_resolution / i_resolution;
	
	// Calculate the top left corner of the nearest 4 pixels
	vec2 flooredPos = floor(tracedPos - 0.5) + 0.5;

	vec2 t = fract(tracedPos);

	vec4 tex11 = texture2D(tex_target, flooredPos / i_target_resolution) * i_dissipation; // Top left
	vec4 tex12 = texture2D(tex_target, (flooredPos + vec2(1, 0)) / i_target_resolution.xy) * i_dissipation; // Top right
	vec4 tex21 = texture2D(tex_target, (flooredPos + vec2(0, 1)) / i_target_resolution.xy) * i_dissipation; // Bottom left
	vec4 tex22 = texture2D(tex_target, (flooredPos + vec2(1, 1)) / i_target_resolution.xy) * i_dissipation; // Bottom right

	return mix(mix(tex11, tex12, t.x), mix(tex21, tex22, t.x), t.y);
}

void main() {
	vec2 targetPos = gl_FragCoord.xy / i_target_resolution.xy;
	float obstacle = texture2D(tex_obstacles, targetPos).x;
	vec4 outVal;
	if(obstacle > 0) {
		outVal = vec4(0);
	}
	else {
		outVal = inner(targetPos);
	}

	fragColor = outVal;
}

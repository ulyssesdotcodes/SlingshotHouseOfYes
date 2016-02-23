#version 330 core

uniform vec3 i_resolution;
uniform vec3 i_targetResolution;

uniform sampler3D tex_velocity;
uniform sampler3D tex_target;
uniform float i_dt;

in float gLayer;
out vec4 fragColor;

void main() {
	vec3 targetPos = vec3(gl_FragCoord.xy, gLayer);
	vec3 pos = targetPos / i_targetResolution;

	vec3 velocity = texture(tex_velocity, pos).xyz;
	vec3 tracedPos = targetPos - i_dt * velocity;
	
	float dissipation = 0.98;

	//fragColor = texture(tex_target, tracedPos / i_targetResolution);
	vec3 t = fract(tracedPos);
	vec3 flooredPos = floor(tracedPos);
	vec4 tex11 = texture(tex_target, flooredPos / i_targetResolution.xyz) * dissipation; // Top left
	vec4 tex12 = texture(tex_target, (flooredPos + vec3(1, 0, 0)) / i_targetResolution.xyz) * dissipation; // Top right
	vec4 tex21 = texture(tex_target, (flooredPos + vec3(0, 1, 0)) / i_targetResolution.xyz) * dissipation; // Bottom left
	vec4 tex22 = texture(tex_target, (flooredPos + vec3(1, 1, 0)) / i_targetResolution.xyz) * dissipation; // Bottom right

	fragColor = mix(mix(tex11, tex12, t.x), mix(tex21, tex22, t.x), t.y);
}

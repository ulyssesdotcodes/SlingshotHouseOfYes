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

	vec3 velocity = texture(tex_velocity, targetPos / i_targetResolution).xyz;
	vec3 tracedPos = targetPos - i_dt * velocity * i_targetResolution / i_resolution;

	fragColor = texture(tex_target, targetPos / i_targetResolution);
}

#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_velocity;
uniform sampler3D tex_smoke;

uniform float i_dt;

in float gLayer;
out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution.xyz;

	vec4 v = texture(tex_velocity, pos);
	vec2 smoke = texture(tex_smoke, pos).xy; // x is density, y is temperature

	float Fb = (2 * smoke.x - (smoke.y - 0.2)) * i_dt; // buoyancy = (-k*density + (T - T0))
	//v.y += Fb;

	fragColor = v;
}

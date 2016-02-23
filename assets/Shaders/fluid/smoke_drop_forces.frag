#version 330 core

uniform vec3 i_resolution;
uniform sampler3D tex_velocity;
uniform sampler3D tex_smoke;

uniform float i_dt;
uniform float i_time;

in float gLayer;
out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec3 pos = vec3(gl_FragCoord.xy, gLayer) / i_resolution.xyz;

	vec4 v = texture(tex_velocity, pos);
	vec2 smoke = texture(tex_smoke, pos).xy; // x is density, y is temperature

	float Fb = ((smoke.y + 0.2) - 0.25 * smoke.x) * i_dt * 100; // buoyancy = (-k*density + (T - T0))
	v.y += Fb;
	//v.x += 10 * (cos(i_time * 0.25) * cos(i_time * 0.25) - 0.5);
	//v.z += 10 * (sin(i_time * 0.25) * sin(i_time * 0.25) - 0.5);

	fragColor = v;//+ vec4(mod(floor(i_time) * 0.002, 2),0,1,1);
}

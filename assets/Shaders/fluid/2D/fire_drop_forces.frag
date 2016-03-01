#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_velocity;
uniform sampler2D tex_smoke;
uniform sampler2D tex_obstacles;

uniform float i_dt;
uniform float i_time;

out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;
	vec4 obstacles = texture2D(tex_obstacles, pos);

	if(obstacles.x > 0) {
	  fragColor = vec4(0);
	  return;
	}

	vec2 v = texture2D(tex_velocity, pos).xy;
	vec2 smoke = texture2D(tex_smoke, pos).xy; // x is density, y is temperature

	float Fb = ((smoke.y * 0.16 - 0.02) - 0.01 * smoke.x) * i_dt; // buoyancy = (-k*density + (T - T0))
	v.y += Fb;
	//v.x += sin(i_time * 16) * 0.125;

	// Add this line in to move the smoke back and forth a bit
	 //v.x += cos(i_time * 0.25) * cos(i_time * 0.25) - 0.5;
	fragColor = vec4(v, 0, 1);
}

#version 330 core

uniform vec2 i_resolution;
uniform sampler2D tex_velocity;
uniform sampler2D tex_smoke;
uniform sampler2D tex_obstacles;

uniform float i_dt;
uniform float i_time;

uniform vec2 i_smokeDropPos;

out vec4 fragColor;

void main() {
	vec4 outVelocity;
	vec2 pos = gl_FragCoord.xy / i_resolution.xy;
	vec4 obstacles = texture2D(tex_obstacles, pos);

	//if(obstacles.x > 0) {
	//  fragColor = vec4(0);
	//  return;
	//}

	vec2 mSDP = vec2(i_smokeDropPos.x, 1.0 - i_smokeDropPos.y);

	vec2 dropDistance = pos - mSDP;

	vec2 v = texture2D(tex_velocity, pos).xy;
	vec2 smoke = texture2D(tex_smoke, pos).xy; // x is density, y is temperature

	float Fb = ((smoke.y - 0.02) - smoke.x) * i_dt * 32;
	vec2 F = vec2(0, Fb); // buoyancy = (-k*density + (T - T0))

	// Add this line in to move the smoke back and forth a bit
	//F.x += 0.02;
	//F.y += (sin(i_time * 0.25) * sin(i_time * 0.25) - 0.5) * 0.25;

	fragColor = vec4(v + F, 0, 1);
}

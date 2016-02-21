#version 150

out vec4 fragColor;

uniform sampler3D tex_smoke;

uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform vec2 i_resolution;
uniform vec3 i_origin;

vec4 getSmoke(vec3 pos)
{
    return texture(tex_smoke, pos);
}

struct Ray {
    vec3 Origin;
    vec3 Dir;
};

struct AABB {
    vec3 Min;
    vec3 Max;
};

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1)
{
    vec3 invR = 1.0 / r.Dir;
    vec3 tbot = invR * (aabb.Min-r.Origin);
    vec3 ttop = invR * (aabb.Max-r.Origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);
    return t0 <= t1;
}

float randhash(uint seed, float b)
{
    const float InverseMaxInt = 1.0 / 4294967295.0;
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(b * i) * InverseMaxInt;
}

void main()
{
    vec3 Ambient = vec3(0.5);
	float StepSize = sqrt(2) / 32;
	float FocalLength = 1.0f / tan(0.7 / 2);
    vec3 rayDirection;
    rayDirection.xy = 2.0 * gl_FragCoord.xy / i_resolution - 1.0;
    rayDirection.x /= i_resolution.y / i_resolution.x;
	rayDirection.z -= FocalLength;
    rayDirection = (vec4(rayDirection, 0) * ciModelView).xyz;

    Ray eye = Ray( (vec4(i_origin, 0) * ciModelView).xyz, normalize(rayDirection) );
    AABB aabb = AABB(vec3(-1), vec3(1));

    float tnear, tfar;
    IntersectBox(eye, aabb, tnear, tfar);
    if (tnear < 0.0) tnear = 0.0;

    vec3 rayStart = eye.Origin + eye.Dir * tnear;
    vec3 rayStop = eye.Origin + eye.Dir * tfar;
    rayStart = 0.5 * (rayStart + 1.0);
    rayStop = 0.5 * (rayStop + 1.0);

    vec3 pos = rayStart;
    vec3 viewDir = normalize(rayStop-rayStart) * StepSize;
    float T = 1.0;
    vec3 Lo = Ambient;

    float remainingLength = distance(rayStop, rayStart);

    for (int i=0; i < 32 && remainingLength > 0.0;
        ++i, pos += viewDir, remainingLength -= StepSize) {

		vec4 smoke = getSmoke(pos);
        //if (smoke.x <= 0.01) {
        //    continue;
        //}

		vec3 color = hsv2rgb(vec3(smoke.z, 0.7, smoke.x * 2048 + smoke.y * 0.25));

        T *= 1.0 - smoke.x * StepSize;
        //if (T <= 0.01)
        //    break;

        Lo += color * StepSize;
    }

    fragColor.rgb = Lo;
    fragColor.a = 1 - T;

    //FragColor.rgb = Lo;
    //FragColor.a = 1;
}

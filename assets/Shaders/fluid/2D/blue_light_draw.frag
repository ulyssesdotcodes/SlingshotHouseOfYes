#version 330 core

uniform sampler2D tex_smoke;
uniform sampler2D tex_obstacles;

uniform vec2 i_resolution;

uniform vec2 i_volume;

out vec4 fragColor;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 GaussianBlur( sampler2D tex0, vec2 centreUV, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   
                                                                                                                                                                     
    ////////////////////////////////////////////////;
    // Kernel width 7 x 7
    //
    const int stepCount = 2;

	float gWeights[2];
	gWeights[0] = 0.44908;
	gWeights[1] = 0.05092;

	float gOffsets[2];
    gOffsets[0] = 0.53805;
    gOffsets[1] = 2.06278;

    ////////////////////////////////////////////////;

    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           

		vec3 smokeA = texture2D(tex0, centreUV + texCoordOffset).xyz;
		vec3 smokeB = texture2D(tex0, centreUV + texCoordOffset).xyz;

		vec3 colA = hsv2rgb(vec3(smokeA.z, 0.6, smokeA.x * 0.5));
		vec3 colB = hsv2rgb(vec3(smokeB.z, 0.6, smokeB.x * 0.5));
		
        vec3 col =  colA + colB;                                               

        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                

    return colOut;
}                       

void main() {
    vec2 pos = gl_FragCoord.xy / i_resolution.xy;
	vec4 smoke = texture2D(tex_smoke, pos);

	if(smoke.x > 0) {
		vec3 outColor;

		for (int i = 0; i < 7; i++ ) {
		  outColor += GaussianBlur(tex_smoke, vec2(pos.x - (i-2) / i_resolution.x, pos.y), vec2(1 / i_resolution.x, 0));
		  outColor += GaussianBlur(tex_smoke, vec2(pos.x, pos.y - (i-2) / i_resolution.y), vec2(0, 1 / i_resolution.y));
		}

		fragColor = vec4(outColor, 1);
		return;
	}


	fragColor = vec4(vec3(0), 1);
}

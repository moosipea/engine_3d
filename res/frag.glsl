#version 130
precision highp float;

in vec2 uv;
in vec3 ex_Color;
out vec4 ex_FragColor;

uniform vec2 iResolution;
uniform sampler2D textureSampler;

uniform float specular;

// epic dithering
vec3 dither(vec3 col, float dsize, vec2 uv, vec2 frag, float aspect, float b) {
	float hlimit = floor(mix(dsize*aspect, dsize, uv.x));
	float vlimit = floor(mix(dsize, dsize*aspect, uv.y));
	float offset = mod(frag.y, 4.0*vlimit) > 2.0 * vlimit ? 0.0 : dsize / 2.0;
	float d = (mod(frag.x-offset, 2.0*hlimit)>hlimit)||(mod(frag.y+vlimit, 2.0*vlimit)>vlimit) ? 1.0 : 0.0;
	float m = dot(col, vec3(0.299, 0.587, 0.144)) < b ? 1.0 : 0.0;
	return mix(col*1.25, col, d * m);
}

void main(void) {
	vec2 s_uv = vec2(gl_FragCoord.x / iResolution.x, gl_FragCoord.y / iResolution.y);

	// vignette
  float vignette = min(1.0 - sqrt(pow(s_uv.x-0.5, 2.0) + pow(s_uv.y-0.5, 2.0)), 1.0);
	float vignette_strength = 0.0;
	
//	ex_FragColor = vec4(ex_Color*mix(1.0, vignette, vignette_strength), 1.0f);
	vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
	float s = clamp(pow(ex_Color.y, specular), 0.0f, 1.0f);
	vec4 sv = vec4(s, s, s, 1.0f);
	vec3 color = texture(textureSampler, uv).rgb * ex_Color.x + ambient;
	vec3 dithered = dither(color, 3.0, s_uv, gl_FragCoord.xy, iResolution.x / iResolution.y, 0.33);
	
	bool show_normals = false;
	if(show_normals) {
		ex_FragColor = vec4(ex_Color, 1.0);
	} else {
		ex_FragColor = vec4(dithered, 1.0f);
	}
}

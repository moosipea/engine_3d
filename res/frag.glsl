#version 130
precision highp float;

in vec3 ex_Color;
out vec4 ex_FragColor;

uniform vec2 iResolution;

void main(void) {
	vec2 uv = vec2(gl_FragCoord.x / iResolution.x, gl_FragCoord.y / iResolution.y);

	// vignette
  float vignette = min(1.0 - sqrt(pow(uv.x-0.5, 2.0) + pow(uv.y-0.5, 2.0)), 1.0);
	float vignette_strength = 0.0;

	ex_FragColor = vec4(ex_Color*mix(1.0, vignette, vignette_strength), 1.0f);
}

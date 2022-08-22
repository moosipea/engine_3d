#version 130
precision highp float;

in vec4 in_Position;
in vec3 in_Color;

out vec3 ex_Color;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

void main(void) {
	vec4 t =  matProj * matView * matModel * vec4(in_Position.xyz, 1.0f);
	gl_Position = vec4(t.x/t.z, t.y/t.z, t.z/t.z, 1.0f);
	ex_Color = in_Color;
}

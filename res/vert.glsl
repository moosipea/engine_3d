#version 130
precision highp float;

in vec4 in_Position;
in vec3 in_Color;
in vec2 vertex_uv;
in vec3 vertex_normal;

out vec3 ex_Color;
out vec2 uv;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

uniform mat4 matRotTest;

uniform vec3 sunDirection;
uniform vec3 cameraPosition;

void main(void) {
	//gl_Position = matProj * matView * matModel * vec4(in_Position.xyz, 1.0);
	vec4 t = matProj * matView *  matModel * vec4(in_Position.xyz, 1.0f);
	gl_Position = vec4(t.xyz / t.w, 1.0);
	//float l = t.w != 0.0f ? t.w : 1.0f;
	//if(t.w != 0.0) {
		//gl_Position = vec4(t.x/t.w, t.y/t.w, t.z/t.w, 1.0f);
	//}
	// ex_Color = in_Color;
	// probably a bad way of doing this
	vec3 _col = vec3(1.0f, 1.0f, 1.0f);
	_col.x = clamp(dot(normalize(vertex_normal), normalize(sunDirection)), 0.0, 1.0);

	vec3 reflection = normalize(cameraPosition - 2.0f * dot(cameraPosition, normalize(vertex_normal)) * vertex_normal);  
	_col.y = dot(normalize(vertex_normal), reflection);
	
	bool show_normals = false;
	if(show_normals) {
		ex_Color = vertex_normal;	
	} else {
		ex_Color = _col;
	}
	uv = vertex_uv;
}

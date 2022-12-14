#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "text.h"
#include "files.h"
#include "maths.h"

class Shader {
	public:
		Shader(std::string vertex_str, std::string frag_str) {load_shader(vertex_str, frag_str);}
		Shader(Shader& src) : program(src.get_program()) {}
		Shader() {}
		
		GLuint get_program() {
			return this->program;
		}

		void load_from_path(std::string vertex_path, std::string frag_path) {
			load_shader(load_string(vertex_path), load_string(frag_path));
		}

		void setUniform1f(std::string uniform, float x) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniform1f(loc, x);
		}
		
		void setUniform2f(std::string uniform, vec2f x) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniform2f(loc, x.getX(), x.getY());
		}

		void setUniform3f(std::string uniform, vec3f x) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniform3f(loc, x.getX(), x.getY(), x.getZ());
		}
		
		void setUniform4f(std::string uniform, vec4f x) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniform4f(loc, x.getX(), x.getY(), x.getZ(), x.getW());
		}

		void setUniformMatrix2f(std::string uniform, mat2x2f m) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniformMatrix2fv(loc, 1, GL_FALSE, &m.m[0][0]);
		}

		void setUniformMatrix3f(std::string uniform, mat3x3f m) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniformMatrix3fv(loc, 1, GL_FALSE, &m.m[0][0]);
		}

		void setUniformMatrix4f(std::string uniform, mat4x4f m) {
			GLint loc = glGetUniformLocation(this->program, uniform.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m.m[0][0]);
		}

		void enable() {
			glUseProgram(this->program);
		}

		void disable() {
			glUseProgram(0);
		}

	private:
		Logger log;
		GLuint program;
		std::string vertex, fragment;

		void load_shader(std::string vert_src, std::string frag_src) {
			GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
			GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

			const char* vert_cstr = vert_src.c_str();
			const char* frag_cstr = frag_src.c_str();

			GLint result = GL_FALSE;
			int log_length;
			log.info("Compiling vertex shader");
			glShaderSource(vert_shader, 1, &vert_cstr, NULL);
			glCompileShader(vert_shader);
			log.info("Checking vertex shader");
			glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
			glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &log_length);
			std::vector<GLchar> vert_error(log_length > 1 ? log_length : 1);
			glGetShaderInfoLog(vert_shader, log_length, NULL, &vert_error[0]);
			for(GLchar c : vert_error) {std::cout << c;}
			
			log.info("Compiling fragment shader");
			glShaderSource(frag_shader, 1, &frag_cstr, NULL);
			glCompileShader(frag_shader);
			log.info("Checking fragment shader");
			glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
			glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &log_length);
			std::vector<GLchar> frag_error(log_length > 1 ? log_length : 1);
			glGetShaderInfoLog(frag_shader, log_length, NULL, &frag_error[0]);
			for(GLchar c : frag_error) {std::cout << c;}

			log.info("Linking program");
			this->program = glCreateProgram();
			glAttachShader(this->program, vert_shader);
			glAttachShader(this->program, frag_shader);

			// Well, shit
			glBindAttribLocation(this->program, 0, "in_Position");
			glBindAttribLocation(this->program, 1, "in_Color");
			glBindAttribLocation(this->program, 2, "vertex_uv");
			glBindAttribLocation(this->program, 3, "vertex_normal");

			glLinkProgram(this->program);
			log.info("Checking program");
			glGetProgramiv(this->program, GL_LINK_STATUS, &result);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &log_length);
			std::vector<GLchar> program_error(log_length > 1 ? log_length : 1);
			glGetProgramInfoLog(this->program, log_length, NULL, &program_error[0]);
			for(GLchar c : program_error) {std::cout << c;}
			
			glDeleteShader(vert_shader);
			glDeleteShader(frag_shader);
		}
};

class DefaultShader : public Shader {
	public:
		DefaultShader() {
			this->vert_shader = 
				"#version 130\n"
				"precision highp float;\n"
				"in vec4 in_Position;\n"
				"in vec3 in_Color\n;"
				"in vec2 in_UV;\n"
				"in vec in_Normal;\n"
				"out vec3 out_Color;\n"
				"out vec2 out_UV;\n"
				"out float out_Light;\n"
				"uniform mat4 uMatProj;\n"
				"uniform mat4 uMatView;\n"
				"uniform mat4 uMatModel;\n"
				"uniform vec3 uSunDirection;\n"
				"uniform vec3 uCameraPosition;\n"
				"void main(void) {\n"
				"vec4 t = uMatProj * uMatView * uMatModel * vec4(in_Position.xyz, 1.0f);\n"
				"float l = t.w != 0.0f ? t.w : 1.0f;\n"
				"gl_Position = vec4(t.x/l, t.y/l, t.z/l, 1.0f);\n"
				"out_Light = dot(normalize(in_Normal), normalize(uSunDirection));\n"
				"out_Color = in_Color;\n"
				"out_UV = in_UV"
				"}";
			this->frag_shader = 
				"#version 130\n"
				"precision highp float;\n"
				"in vec2 out_UV;\n"
				"in vec3 out_Color;\n"
				"in float out_Light;\n"
				"out vec4 out_FragColor;\n"
				"uniform vec2 uResolution;\n"
				"uniform sampler2D uTextureSampler;\n"
				"uniform vec3 uAmbientColor;\n"
				"uniform float uBrightness;\n"
				"uniform float uContrast;\n"
				"uniform float uSaturation;\n"
				"uniform float uHue;\n"
				"void main(void) {\n"
				"vec4 a4 = vec4(uAmbientColor.rgb, 1.0f);\n"
				"out_FragColor = vec4(texture(uTextureSampler).rgb, 1.0f) * out_Light + a4;\n"
				"}";
			Shader(this->vert_shader, this->frag_shader);
		}
	
	void setAmbient(vec3f ambient_color) {
		this->setUniform3f("uAmbientColor", ambient_color);
	}

	void setBrightness(float brightness) {
		this->setUniform1f("uBrightness", brightness);
	}
	void setContrast(float contrast) {
		this->setUniform1f("uContrast", contrast);
	}
	void setSaturation(float saturation) {
		this->setUniform1f("uSaturation", saturation);
	}
	void setHue(float hue) {
		this->setUniform1f("uHue", hue);
	}

	void setViewMatrix(mat4x4f mat_view) {
		this->setUniformMatrix4f("uMatView", mat_view);
	}
	void setProjMatrix(mat4x4f mat_proj) {
		this->setUniformMatrix4f("uMatProj", mat_proj);
	}
	void setModelMatrix(mat4x4f mat_model) {
		this->setUniformMatrix4f("uMatModel", mat_model);
	}

	private:
		std::string vert_shader;
		std::string frag_shader;
};

#endif

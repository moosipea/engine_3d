#ifndef RENDERER_H
#define RENDERER_H
#include "maths.h"
#include "shader.h"
#include "entity.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <SDL2/SDL_video.h>

class Renderer {
	public:
		Renderer(SDL_Window* window, SDL_GLContext &context) {}
		Renderer() {}
		Renderer(SDL_Window *window, SDL_GLContext &context, int width, int height) {
			this->window = window;
			this->context = &context;
			this->width = width;
			this->height = height;
		}

		~Renderer() {
			glDeleteVertexArrays(1, &vertex_array_ID);
		}

		void init() {
			glFrontFace(GL_CCW); // IMPORTANT
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glGenVertexArrays(1, &this->vertex_array_ID);	
			glBindVertexArray(vertex_array_ID);
			std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;
		}

		void clear(float r, float g, float b) {
			glViewport(0, 0, this->width, this->height); // should this be here?
			glClearColor(r, g, b, 1.0f);
			glDepthFunc(GL_GREATER);
			glClearDepth(0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		// optimize this shit
		/*void render(Model model) {
			int p_count = model.getModelData().getVertexCount() * 3;
			GLfloat vertex_buffer_data[p_count];
			GLfloat color_buffer_data[p_count];
			GLfloat uv_buffer_data[model.getModelData().getVertexCount() * 2];
			for(int i = 0; i < p_count; i++) {
				vertex_buffer_data[i] = model.getModelData().getVertices()[i];
				color_buffer_data[i] = model.getModelData().getColors()[i];
			}
			for(int j = 0; j < model.getModelData().getVertexCount() * 2; j++) {
				uv_buffer_data[j] = model.getModelData().getUVs()[j];
			}

			// vertices
			GLuint vertex_buffer;
			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// vertex colors
			GLuint color_buffer;
			glGenBuffers(1, &color_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// uv coords
			GLuint uv_buffer;
			glGenBuffers(1, &uv_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(this->draw_mode, 0, model.getModelData().getVertexCount() * 3);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}*/

		void render(TexturedModel m) {
			GLuint vertex_buffer, color_buffer, uv_buffer, normals_buffer;
			m.getModelData()->getBuffers(vertex_buffer, color_buffer, uv_buffer, normals_buffer);
			
			glBindTexture(GL_TEXTURE_2D, m.getTexture());

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(this->draw_mode, 0, m.getModelData()->getVertexCount() * 3);
			
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
		}

		void flip() {
			SDL_GL_SwapWindow(this->window);
		}

	private:
		SDL_Window* window = NULL;
		SDL_GLContext* context = NULL;
		GLuint vertex_array_ID;
		GLuint draw_mode = GL_TRIANGLES;
		int width, height;
};

// Considering switching to vec3f
class PerspectiveCamera : public Entity {
	public:
		PerspectiveCamera() : Entity(0, 0, 0) {}
		PerspectiveCamera(float x, float y, float z) : Entity(x, y, z) {}
		
		// IMPORTANT has to be called before doing anything else
		void configureFustrum(float fov, int width, int height, float near, float far) {
			this->mat_projection = gen_perspective_matrix(fov, (float)width/(float)height, near, far);
		}
		
		void move(float vx, float vy, float vz) {
			this->x += vx;
			this->y += vy;
			this->z += vz;
		}

		void move_forward(float v) {
			this->x += v * sin(rad(this->ry));
			this->z += v * cos(rad(this->ry));
		}

		void move_side(float v) {
			this->x += v * sin(rad(this->ry + 90.0f));
			this->z += v * cos(rad(this->ry + 90.0f));
		}

		/*void rotate(float horizontal, float vertical) {
			this->rx += vertical * cos(rad(horizontal));
			this->ry += horizontal;
			this->rz += vertical * sin(rad(horizontal));
		}*/
		void rotate(float x, float y, float z) {
			this->rx += x;
			this->ry += y;
			this->rz += z;
		}

		void lookAt(float x, float y, float z) {
			// TODO:
			// 	make rotation matrices
			// 	implement this
		}
		
		// Assumes uniforms matView & matProj
		void loadMatrices(Shader &shader,
											std::string view_uniform = "matView",
											std::string proj_uniform = "matProj") {
			this->recalculate_matrices();
			shader.setUniformMatrix4f(view_uniform, this->mat_view);
			shader.setUniformMatrix4f(proj_uniform, this->mat_projection);
			//shader.setUniformMatrix4f("matRotTest",gen_rotation_matrix_euler(this->rx,this->ry,this->rz));
		}
		
		// why does this not work??
		vec3f getPositionVector() {
			vec3f vec(this->x, this->y, this->z);
			return vec;
		}

	private:	
		void recalculate_matrices() {
			this->mat_view = gen_translation_matrix(-this->x, -this->y, -this->z) * gen_rotation_matrix_euler(this->rx, this->ry, this->rz);
			//this->mat_view = gen_rotation_matrix_euler(this->rx, this->ry, this->rz) * gen_translation_matrix(-this->x, -this->y, -this->z);
		}
		
		// Maybe move these into the constructor later?
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;

		mat4x4f mat_view;
		mat4x4f mat_projection;
};
#endif

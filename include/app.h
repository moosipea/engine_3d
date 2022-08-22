#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <bits/chrono.h>
#include <chrono>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "renderer.h"

class App {
	public:
		App() {}
			
		virtual bool update(float delta) = 0;
		virtual bool create() = 0;

		void construct(int width, int height, std::string title) {
			this->width = width;
			this->height = height;
			this->title = title;
			
			this->window = SDL_CreateWindow(this->title.c_str(), 0, 0,
																			this->width, this->height,
																			this->flags);
			if(!this->window)
				return;

			this->gl_context = SDL_GL_CreateContext(this->window);
			if(!this->gl_context)
				return;

			GLenum err = glewInit();
			if(err != GLEW_OK || !GL_VERSION_2_1)
				return;
			
			this->renderer = Renderer(window, gl_context, this->width, this->height);
			this->renderer.init();

			if(create())
				this->start_loop();
		}

		inline Renderer* getRenderer() {
			return &this->renderer;
		}

		inline int getWidth() {
			return this->width;
		}

		inline int getHeight() {
			return this->height;
		}

		inline float getFPS() {
			return 1.0f / this->delta;
		}

		inline void setTitle(std::string title) {
			SDL_SetWindowTitle(this->window, title.c_str());
		}

		inline std::string getTitle() {
			return this->title;	
		}

	private: 

		void start_loop() {
			std::chrono::high_resolution_clock t;

			while(this->is_running) {
				
				auto t_start = t.now();

				SDL_Event e;
				
				while(SDL_PollEvent(&e)) {
					if(e.type == SDL_QUIT) {
						this->is_running = false;
						break;
					}
				}
				
				//this->renderer.clear(0.0f, 0.25f, 0.25f);	
				this->renderer.clear(0.0f, 0.0f, 0.0f);

				// hidden update() call
				if(!update(this->delta)) this->is_running = false;
				
				this->renderer.flip();
				
				auto t_end = t.now();
				this->delta = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count() / 1000000.0f;;
			}
		}
		
		float delta = 1.0f;
		bool is_running = true;

		int width, height;
		std::string title;
		
		SDL_Window* window = NULL;
		SDL_GLContext gl_context = NULL;
		uint32_t flags = SDL_WINDOW_OPENGL;

		Renderer renderer;
};

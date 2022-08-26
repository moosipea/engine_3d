#include "include/maths.h"
#include "include/renderer.h"
#include "include/files.h"
#include "include/shader.h"
#include "include/app.h"
#include "include/input.h"
#include <iostream>
#include <string>

class tApp : public App {
	bool create() {
		
		// vertices for a cube
/*		std::vector<float> vertices = {
			-1.0f,-1.0f,-1.0f,
    	-1.0f,-1.0f, 1.0f,
    	-1.0f, 1.0f, 1.0f,
    	1.0f, 1.0f,-1.0f,
    	-1.0f,-1.0f,-1.0f,
    	-1.0f, 1.0f,-1.0f,
    	1.0f,-1.0f, 1.0f,
    	-1.0f,-1.0f,-1.0f,
    	1.0f,-1.0f,-1.0f,
    	1.0f, 1.0f,-1.0f,
    	1.0f,-1.0f,-1.0f,
    	-1.0f,-1.0f,-1.0f,
    	-1.0f,-1.0f,-1.0f,
    	-1.0f, 1.0f, 1.0f,
    	-1.0f, 1.0f,-1.0f,
    	1.0f,-1.0f, 1.0f,
    	-1.0f,-1.0f, 1.0f,
    	-1.0f,-1.0f,-1.0f,
    	-1.0f, 1.0f, 1.0f,
    	-1.0f,-1.0f, 1.0f,
    	1.0f,-1.0f, 1.0f,
    	1.0f, 1.0f, 1.0f,
    	1.0f,-1.0f,-1.0f,
    	1.0f, 1.0f,-1.0f,
    	1.0f,-1.0f,-1.0f,
    	1.0f, 1.0f, 1.0f,
    	1.0f,-1.0f, 1.0f,
    	1.0f, 1.0f, 1.0f,
    	1.0f, 1.0f,-1.0f,
    	-1.0f, 1.0f,-1.0f,
    	1.0f, 1.0f, 1.0f,
    	-1.0f, 1.0f,-1.0f,
    	-1.0f, 1.0f, 1.0f,
    	1.0f, 1.0f, 1.0f,
   		-1.0f, 1.0f, 1.0f,
    	1.0f,-1.0f, 1.0f			
		};

		std::vector<float> colors = {
			0.583f,  0.771f,  0.014f,
    	0.609f,  0.115f,  0.436f,
    	0.327f,  0.483f,  0.844f,
    	0.822f,  0.569f,  0.201f,
    	0.435f,  0.602f,  0.223f,
    	0.310f,  0.747f,  0.185f,
    	0.597f,  0.770f,  0.761f,
    	0.559f,  0.436f,  0.730f,
    	0.359f,  0.583f,  0.152f,
    	0.483f,  0.596f,  0.789f,
    	0.559f,  0.861f,  0.639f,
    	0.195f,  0.548f,  0.859f,
    	0.014f,  0.184f,  0.576f,
    	0.771f,  0.328f,  0.970f,
    	0.406f,  0.615f,  0.116f,
    	0.676f,  0.977f,  0.133f,
    	0.971f,  0.572f,  0.833f,
    	0.140f,  0.616f,  0.489f,
  		0.997f,  0.513f,  0.064f,
    	0.945f,  0.719f,  0.592f,
    	0.543f,  0.021f,  0.978f,
    	0.279f,  0.317f,  0.505f,
    	0.167f,  0.620f,  0.077f,
    	0.347f,  0.857f,  0.137f,
    	0.055f,  0.953f,  0.042f,
    	0.714f,  0.505f,  0.345f,
    	0.783f,  0.290f,  0.734f,
    	0.722f,  0.645f,  0.174f,
    	0.302f,  0.455f,  0.848f,
    	0.225f,  0.587f,  0.040f,
    	0.517f,  0.713f,  0.338f,
    	0.053f,  0.959f,  0.120f,
    	0.393f,  0.621f,  0.362f,
    	0.673f,  0.211f,  0.457f,
    	0.820f,  0.883f,  0.371f,
    	0.982f,  0.099f,  0.879f	
		};

    std::vector<float> uv_coords = {
      0.000059f, 1.0f-0.000004f,
      0.000103f, 1.0f-0.336048f,
      0.335973f, 1.0f-0.335903f,
      1.000023f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.336024f, 1.0f-0.671877f,
      0.667969f, 1.0f-0.671889f,
      1.000023f, 1.0f-0.000013f,
      0.668104f, 1.0f-0.000013f,
      0.667979f, 1.0f-0.335851f,
      0.000059f, 1.0f-0.000004f,
      0.335973f, 1.0f-0.335903f,
      0.336098f, 1.0f-0.000071f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.336024f, 1.0f-0.671877f,
      1.000004f, 1.0f-0.671847f,
      0.999958f, 1.0f-0.336064f,
      0.667979f, 1.0f-0.335851f,
      0.668104f, 1.0f-0.000013f,
      0.335973f, 1.0f-0.335903f,
      0.667979f, 1.0f-0.335851f,
      0.335973f, 1.0f-0.335903f,
      0.668104f, 1.0f-0.000013f,
      0.336098f, 1.0f-0.000071f,
      0.000103f, 1.0f-0.336048f,
      0.000004f, 1.0f-0.671870f,
      0.336024f, 1.0f-0.671877f,
      0.000103f, 1.0f-0.336048f,
      0.336024f, 1.0f-0.671877f,
      0.335973f, 1.0f-0.335903f,
      0.667969f, 1.0f-0.671889f,
      1.000004f, 1.0f-0.671847f,
      0.667979f, 1.0f-0.335851f 
    };*/

		 // this->m = Model(0.0f, 0.0f, 0.0f, vertices, colors);
   // this->m = Model(0.0f, 0.0f, 0.0f, vertices);
     // this->m.setUVs(uv_coords);
    //this->m2 = Model(3.0f, 0.0f, 0.0f, vertices);
    
    //this->m = Model(0.0f, 0.0f, 0.0f, "./res/stall.obj");

    //this->tex = BitmapTexture("./res/stallTexture.bmp");
    
		this->meshes.push_back(TexturedModel(0.0f, 0.0f, 5.0f, "./res/cube.obj", "./res/grid.bmp"));
		this->meshes.push_back(TexturedModel(15.0f, 0.0f, 5.0f, "./res/Rickmay.obj", "./res/grid.bmp"));

   // this->meshes.push_back(this->m);
    //this->meshes.push_back(this->m2);

		this->s.load_from_path("./res/vert.glsl", "./res/frag.glsl");
		this->resolution = vec2f((float)getWidth(), (float)getHeight());
		
		this->cam = PerspectiveCamera(0.0f, 0.0f, 3.0f);
		this->cam.configureFustrum(60.0f, getWidth(), getHeight(), 0.1f, 1000.0f);

		//this->mat_proj = gen_perspective_matrix(45.0f, (float)getWidth()/(float)getHeight(), 0.1f, 100.0f);
		//this->mat_model = gen_identity_matrix();
		this->mat_model = gen_translation_matrix(3.0f, 0.0f, 0.0f);
		//this->mat_view = gen_scaling_matrix(0.6f, 0.6f, 0.6f) * gen_translation_matrix(0.0f, 0.0f, -1.0f);
			
		this->sun_dir = vec3f(-1.0f, 1.0f, -0.5f);

		this->s.enable();

		return true;
	}

	bool update(float delta) {

    //this->s.enable();

		if(key_held(Keys::W))
      this->cam.move_forward(-2.0f * delta);
		if(key_held(Keys::S))
      this->cam.move_forward(2.0f * delta);
		if(key_held(Keys::A))
      this->cam.move_side(2.0f * delta);
		if(key_held(Keys::D))
      this->cam.move_side(-2.0f * delta);
		if(key_held(Keys::R))
			this->cam.move(0.0, -2.0 * delta, 0.0);
		if(key_held(Keys::F))
			this->cam.move(0.0, 2.0 * delta, 0.0);
    
    // handle rotation
    if(key_held(Keys::LEFT))
			this->cam.rotate(0.0, -45.0 * delta, 0.0);
    if(key_held(Keys::RIGHT))
			this->cam.rotate(0.0, 45.0 * delta, 0.0);
    if(key_held(Keys::UP))
			this->cam.rotate(-45.0 * delta, 0.0, 0.0);
    if(key_held(Keys::DOWN))
			this->cam.rotate(45.0 * delta, 0.0, 0.0);
		if(key_held(Keys::E))
			this->cam.rotate(0.0, 0.0, -45.0 * delta);
		if(key_held(Keys::Q))
			this->cam.rotate(0.0, 0.0, 45.0 * delta);

    setTitle(getTitle() + " fps: " + std::to_string(getFPS()) + " | specular: " + std::to_string(this->specular));
		this->s.setUniform2f("iResolution", this->resolution);
		this->s.setUniformMatrix4f("matModel", this->mat_model);
		this->s.setUniform3f("sunDirection", this->sun_dir);

		vec3f camPos(this->cam.x, this->cam.y, this->cam.z);
		this->s.setUniform3f("cameraPosition", camPos);

		this->s.setUniform1f("specular", this->specular);
		/*this->s.setUniformMatrix4f("matView", this->mat_view);
		this->s.setUniformMatrix4f("matProj", this->mat_proj);*/
	
		this->cam.loadMatrices(this->s);
		
		for(auto& model : this->meshes) {
			model.transform(0.0, 0.0, 0.0, 0.0, 15.0*delta, 0.0);
			this->s.setUniformMatrix4f("matModel", model.getMatrix());
			getRenderer()->render(model);
		}
		return true;
	}

	private:
    BitmapTexture tex;
		PerspectiveCamera cam;	
		TexturedModel m;
    std::vector<TexturedModel> meshes;
		Shader s;
		vec3f sun_dir;
		vec2f resolution;
		mat4x4f mat_proj, mat_model, mat_view;
		float specular = 20.0f;
};

int main(int argc, char **argv) {
	
	tApp app;

	app.construct(1280, 720, "Engine v3");

	return 0;
}

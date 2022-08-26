#include <sstream>
#include <string>
#include <vector>
#include "maths.h"
#include "files.h"

class ModelData {
	public:
		ModelData() {}
		ModelData(std::vector<float> vertices, std::vector<float> colors) {
			for(auto& v : vertices) {this->vertices.push_back(v);}
			for(auto& c : colors) {this->vertex_colors.push_back(c);}
			this->vertex_count = this->vertices.size() / 3;
		}
		ModelData(std::vector<float> vertices) {
			for(auto& v : vertices) {this->vertices.push_back(v);}
			this->vertex_count = this->vertices.size() / 3;
		}

		std::vector<float> getVertices() {
			return this->vertices;
		}

		std::vector<float> getColors() {
			return this->vertex_colors;
		}

		std::vector<float> getUVs() {
			return this->uv_coords;
		}

		int getVertexCount() {
			return this->vertex_count;
		}

		// TEMP
		void genColors() {
			this->vertex_colors.clear();
			for(int i = 0; i < this->vertex_count * 3; i++) {
				this->vertex_colors.push_back(std::max(std::min((this->vertices[i] + 1.0f) / 2.0f, 1.0f), 0.0f));
			}
		}

		void setUVs(std::vector<float> uvs) {
			this->uv_coords = std::vector<float>(uvs);
		}

		void load_OBJ(std::string path) {
			std::cout << "Loading " << path << std::endl;
			this->vertices.clear();
			this->vertex_colors.clear();
			this->uv_coords.clear();

			std::vector<float> vertices_t, uv_coords_t, normals_t;
			std::vector<int> vertex_indices, uv_indices, normal_indices;

			std::string obj_src = load_string(path);
			std::vector<std::string> lines = split_string(obj_src, "\n");

			for(auto& l : lines) {
				std::vector<std::string> line = split_string(l);
				if(line.size() == 0)
					continue;
				
				if(line[0] == "v") {
					vertices_t.push_back(std::stof(line[1]));
					vertices_t.push_back(std::stof(line[2]));
					vertices_t.push_back(std::stof(line[3]));
				} else if(line[0] == "vt") {
					uv_coords_t.push_back(std::stof(line[1]));
					uv_coords_t.push_back(std::stof(line[2]));
				} else if(line[0] == "vn") {
					normals_t.push_back(std::stof(line[1]));
					normals_t.push_back(std::stof(line[2]));
					normals_t.push_back(std::stof(line[3]));
				} else if(line[0] == "f") {
					// IMPORTANT
					// indices are stored as vertex_index/uv_index/normal_index
					std::vector<std::string> sec_1 = split_string(line[1], "/");
					std::vector<std::string> sec_2 = split_string(line[2], "/");
					std::vector<std::string> sec_3 = split_string(line[3], "/");
					vertex_indices.push_back(std::stoi(sec_1[0]));
					vertex_indices.push_back(std::stoi(sec_2[0]));
					vertex_indices.push_back(std::stoi(sec_3[0]));

					uv_indices.push_back(std::stoi(sec_1[1]));
					uv_indices.push_back(std::stoi(sec_2[1]));
					uv_indices.push_back(std::stoi(sec_3[1]));

					normal_indices.push_back(std::stoi(sec_1[2]));
					normal_indices.push_back(std::stoi(sec_2[2]));
					normal_indices.push_back(std::stoi(sec_3[2]));
				}
			}

			//for(auto& i : vertex_indices) {
			for(int i = 0; i < vertex_indices.size(); i++) {
				this->vertices.push_back(vertices_t[3 * (vertex_indices[i] - 1) + 0]);
				this->vertices.push_back(vertices_t[3 * (vertex_indices[i] - 1) + 1]);
				this->vertices.push_back(vertices_t[3 * (vertex_indices[i] - 1) + 2]);
			}
			
			//for(auto& i : uv_indices) {
			for(int j = 0; j < uv_indices.size(); j++) {
				this->uv_coords.push_back(uv_coords_t[2 * (uv_indices[j] - 1) + 0]);
				this->uv_coords.push_back(uv_coords_t[2 * (uv_indices[j] - 1) + 1]);
			}

			for(int k = 0; k < normal_indices.size(); k++) {
				this->normals.push_back(normals_t[3 * (normal_indices[k] - 1) + 0]);
				this->normals.push_back(normals_t[3 * (normal_indices[k] - 1) + 1]);
				this->normals.push_back(normals_t[3 * (normal_indices[k] - 1) + 2]);
			}
			
			this->vertex_count = this->vertices.size() / 3;
			std::cout << this->vertex_count << std::endl;
			this->genColors();
		}
		
		// prepare the buffers
		void prepare() {
			GLfloat vertex_data[this->vertices.size()];
			GLfloat color_data[this->vertex_colors.size()];
			GLfloat uv_data[this->uv_coords.size()];
			GLfloat normals_data[this->normals.size()];
			
			std::copy(this->vertices.begin(), this->vertices.end(), vertex_data);
			std::copy(this->vertex_colors.begin(), this->vertex_colors.end(), color_data);
			std::copy(this->uv_coords.begin(), this->uv_coords.end(), uv_data);
			std::copy(this->normals.begin(), this->normals.end(), normals_data);

			glGenBuffers(1, &this->vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
			
			glGenBuffers(1, &this->color_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
			
			glGenBuffers(1, &this->uv_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);

			glGenBuffers(1, &this->normals_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, this->normals_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(normals_data), normals_data, GL_STATIC_DRAW);
		}

		void getBuffers(GLuint &vert_dest, GLuint &col_dest, GLuint &uv_dest, GLuint &norm_dest) {
			vert_dest = this->vertex_buffer;
			col_dest = this->color_buffer;
			uv_dest = this->uv_buffer;
			norm_dest = this->normals_buffer;
		}

	private:
		std::vector<float> vertices;
		std::vector<float> vertex_colors;
		std::vector<float> uv_coords;
		std::vector<float> normals;
		int vertex_count;
		
		GLuint vertex_buffer, color_buffer, uv_buffer, normals_buffer;

		std::vector<std::string> split_string(std::string src, std::string delimiter = " ") {
			std::vector<std::string> result;
			int start = 0;
			int end = src.find(delimiter);
			while(end != -1) {
				result.push_back(src.substr(start, end - start));
				start = end + delimiter.size();
				end = src.find(delimiter, start);
			}
			result.push_back(src.substr(start, end - start));
			return result;
		}
};

class Entity {
	public:
		Entity(float x, float y, float z) : x(x), y(y), z(z) {}
		float x, y, z;
};

class Model : public Entity {
	public:
		Model() : Entity(0, 0, 0) {}
		Model(float x, float y, float z, std::vector<float> vertices, std::vector<float> colors) : Entity(x, y, z) {
			this->data = ModelData(vertices, colors);
			this->data.prepare();
		}
		Model(float x, float y, float z, std::vector<float> vertices) : Entity(x, y, z) {
			this->data = ModelData(vertices);
			this->data.genColors();
			this->data.prepare();
		}
		Model(float x, float y, float z, std::string obj_path) : Entity(x, y, z) {
			this->data.load_OBJ(obj_path);
			this->data.prepare();
		}
		
		ModelData* getModelData() {
			return &this->data;
		}

		void setUVs(std::vector<float> uvs) {
			this->data.setUVs(uvs);
		}

	private:
		ModelData data;
};

class TexturedModel : public Model {
	public:
		TexturedModel() {}
		TexturedModel(float x, float y, float z, std::string model_path, std::string texture_path) : Model(x, y, z, model_path) {
			this->texture = BitmapTexture(texture_path);
			this->rx = 180.0;
			this->ry = 45.0;
			this->rz = 0.0;
			this->model_matrix = gen_translation_matrix(-x, -y, -z) * gen_rotation_matrix_euler(this->rx, this->ry, this->rz);
		}

		GLuint getTexture() {
			return this->texture.getTexture();
		}

		mat4x4f getMatrix() {
			return this->model_matrix;
		}
		
		// this.. does not work??
		void transform(float x, float y, float z, float rx, float ry, float rz) {
			this->x += x;
			this->y += y;
			this->z += z;
			this->rx += rx;
			this->ry += ry;
			this->rz += rz;
			this->model_matrix = gen_translation_matrix(-this->x, -this->y, -this->z) * gen_rotation_matrix_euler(this->rx, this->rx, this->rz);
		}

	private:
		BitmapTexture texture;
		mat4x4f model_matrix;
		float rx, ry, rz;
};

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

		std::vector<float> getVertices() {
			return this->vertices;
		}

		std::vector<float> getColors() {
			return this->vertex_colors;
		}

		int getVertexCount() {
			return this->vertex_count;
		}

	private:
		std::vector<float> vertices;
		std::vector<float> vertex_colors;
		int vertex_count;
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
		}
		
		ModelData getModelData() {
			return this->data;
		}

	private:
		ModelData data;
};

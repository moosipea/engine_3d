#ifndef MATHS_H
#define MATHS_H
#include <cmath>
#include <functional>

template<typename T, size_t D>
struct vec_generic {
	T v[D] = {(T)0};

	vec_generic(T values...) {for(int i = 0; i < D; i++) {this->v[i] = values; values++;}}

	vec_generic(vec_generic<T, D>& src) {for(int j = 0; j < D; j++) {this->v[j] = src.v[j];}}

	vec_generic() {}
	
	T magnitude() {T s = (T)0; vmap([=, &s] (T x) {s += x*x;}); return std::sqrt(s);}

	vec_generic<T, D> normalize() {return map([&, this] (T x) {return x / magnitude();});}

	T dot(vec_generic<T, D>& other, bool auto_normalize = false) {
		T product;
		if(auto_normalize) {
			for(int k = 0; k < D; k++) {product += this->normalize().v[k] * other.normalize().v[k];}
			return product;
		}
		for(int k = 0; k < D; k++) {product += this->v[k] * other.v[k];}
		return product;
	}

	vec_generic<T, D> map(const std::function<T(T)>& f) {
		vec_generic<T, D> product;
		for(int c = 0; c < D; c++) {product.v[c] = f(this->v[c]);}
		return product;
	}

	void vmap(const std::function<void(T)>& f) {for(T& c : this->v) {f(c);}}
		
	T getX() 		 {return this->v[0];}
	T getY() 		 {return this->v[1];}
	T getZ() 		 {return this->v[2];}
	T getW() 		 {return this->v[3];}
	T get(int a) {return this->v[a];}

	inline vec_generic<T, D> operator + (const vec_generic<T, D>& rhs) {
		vec_generic<T, D> product;
		for(int c = 0; c < D; c++) {product.v[c] = this->get(c) + rhs.get(c);}
		return product;
	}

	inline vec_generic<T, D> operator - (const vec_generic<T, D>& rhs) {
		vec_generic<T, D> product;
		for(int c = 0; c < D; c++) {product.v[c] = this->get(c) - rhs.get(c);}
		return product;
	}

	inline vec_generic<T, D> operator * (const float& scalar) {
		vec_generic<T, D> product(this);
		product.map([&, scalar] (T x) {return x * scalar;});
	}

	inline vec_generic<T, D> operator / (const float& scalar) {
		vec_generic<T, D> product(this);
		product.map([&, scalar] (T x) {return x / scalar;});
	}
};

typedef vec_generic<float, 2> vec2f;
typedef vec_generic<float, 3> vec3f;
typedef vec_generic<float, 4> vec4f;
typedef vec_generic<double, 2> vec2d;
typedef vec_generic<double, 3> vec3d;
typedef vec_generic<double, 4> vec4d;



template<typename T, size_t R, size_t C>
struct mat_generic {
	mat_generic() {}
	mat_generic(const mat_generic<T, R, C>& src) {
		for(int rows = 0; rows < R; rows++) {
			for(int cols = 0; cols < C; cols++) {
				m[rows][cols] = src.m[rows][cols];
			}
		}
	
	}
	T m[R][C] = {(T)0};
		
	inline mat_generic<T, R, C> operator*(const mat_generic<T, R, C>& rhs) {
		mat_generic<T, R, C> product;
		size_t bigger = R > C ? R : C;
		for(int y = 0; y < R; y++) {
			for(int x = 0; x < C; x++) {
				for(int i = 0; i < bigger; i++) {
					product.m[y][x] += this->m[y][i<C?i:C]*rhs.m[i<R?i:R][x];			
				}
			}
		}
		return product;
	}

	inline void set(int row, int col, T value) {
		if(row <= R && col <= C)
			this->m[row][col] = value;
	}

};

typedef mat_generic<float, 2, 2> mat2x2f;
typedef mat_generic<float, 3, 3> mat3x3f;
typedef mat_generic<float, 4, 4> mat4x4f;
typedef mat_generic<double, 2, 2> mat2x2d;
typedef mat_generic<double, 3, 3> mat3x3d;
typedef mat_generic<double, 4, 4> mat4x4d;

inline mat4x4f gen_identity_matrix() {
	mat4x4f mat;
	mat.set(0, 0, 1.0f);
	mat.set(1, 1, 1.0f);
	mat.set(2, 2, 1.0f);
	mat.set(3, 3, 1.0f);
	return mat;
}

inline mat4x4f gen_translation_matrix(float x, float y, float z) {
	mat4x4f mat = gen_identity_matrix();
	mat.set(3, 0, x);
	mat.set(3, 1, y);
	mat.set(3, 2, z);
	return mat;
}

inline mat4x4f gen_scaling_matrix(float x, float y, float z) {
	mat4x4f mat;
	mat.set(0, 0, x);
	mat.set(1, 1, y);
	mat.set(2, 2, z);
	mat.set(3, 3, 1.0f);
	return mat;
}

// https://github.com/g-truc/glm/blob/0.9.5/glm/gtc/matrix_transform.inl#L208 
inline mat4x4f gen_perspective_matrix(float fov_y, float aspect, float near, float far) {
	mat4x4f mat;
	float rad = fov_y * (3.14159f / 180.0f);
	float tan_half_fovy = std::tan(rad / 2.0f);
	mat.set(0, 0, 1.0f / (aspect * tan_half_fovy));
	mat.set(1, 1, 1.0f / tan_half_fovy);
	mat.set(2, 2, -(far + near) / (far - near));
	mat.set(2, 3, 1.0f);
	mat.set(3, 2, -(2.0f * far * near) / (far - near));
	return mat;
}
#endif

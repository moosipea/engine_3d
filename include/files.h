#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#ifndef FILES_H
#define FILES_H

inline std::string load_string(std::string path) {
	std::string content;
	std::fstream f(path.c_str(), std::ios::in);
	if(!f.is_open())
		return "";
	std::string l;
	while(!f.eof()) {
		std::getline(f, l);
		content.append(l + "\n");
	}
	f.close();
	return content;
}

// Don't use this lmaoo
class BitmapTexture {
	public:
		BitmapTexture() {}
		BitmapTexture(std::string path) {
			std::cout << "Loading " << path << std::endl;
			FILE* f = fopen(path.c_str(), "rb");
			if(!f) {
				std::cout << "\tFile could not be opened" << std::endl;
				return;
			}

			if(fread(header, 1, 54, f) != 54) {
				std::cout << "\tInvalid BMP file" << std::endl;
				return;
			}

			if(header[0] != 'B' || header[1] != 'M') {
				std::cout << "\tInvalid BMP file" << std::endl;
				return;
			}

			dataPos = *(int*)&(header[0x0a]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);
			
			if(imageSize == 0) {imageSize = width * height * 3;}
			if(dataPos == 0) {dataPos = 54;}

			data = new unsigned char[imageSize];
			fread(data, 1, imageSize, f);
			
			fclose(f);

			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_BGR, GL_UNSIGNED_BYTE, this->data);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// using GL_NEAREST for pixelated look
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		~BitmapTexture() {
			//delete this->data;
		}

		GLuint getTexture() {
			return this->texture;
		}
	
	private:
		GLuint texture;
		unsigned char header[54];
		unsigned int dataPos;
		unsigned int width, height;
		unsigned int imageSize;
		unsigned char* data;
};

#endif

#pragma once

#include <iostream>


class Texture {
public:
	Texture(std::string_view path);
	~Texture();

	void Bind(unsigned int slot = 0)const;
	void Unbind()const;

	inline int GetWidth()const { return textureWidth; }
	inline int GetHeight()const { return textureHeight; }
private:
	unsigned int textureID;
	std::string_view textureFilePath;
	unsigned char* textureLocalBuffer;
	int textureWidth, textureHeight, nrChannels;// byte per pixel
};

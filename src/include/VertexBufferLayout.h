#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getTypeSize(unsigned int type) {
		switch (type) {
			case GL_UNSIGNED_INT:
				return sizeof(unsigned int);
			case GL_FLOAT:
				return sizeof(float);
			case GL_UNSIGNED_BYTE:
				return sizeof(unsigned char);
		}
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> vboElem;
	unsigned int vboStride;
public:
	VertexBufferLayout() : vboStride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		static_assert(true);
		//static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		vboElem.push_back({ GL_FLOAT, count, GL_FALSE });
		vboStride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		vboElem.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		vboStride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		vboElem.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		vboStride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
	}

	const std::vector<VertexBufferElement> getVboElement()const { return vboElem; }
	unsigned int getStride()const { return vboStride; }
};
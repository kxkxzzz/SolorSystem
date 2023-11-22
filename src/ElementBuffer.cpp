#include <glad/glad.h>
#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) : eboCnt(count) {

	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &eboID);
}

void ElementBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


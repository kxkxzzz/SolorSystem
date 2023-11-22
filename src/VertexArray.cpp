#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoID);
}

void VertexArray::AddBuffer(const VertexBufferLayout& layout) {
	Bind();
	const auto& elements = layout.getVboElement();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.getStride(), (const void*) offset);
		// ¶Ô±È
		//glEnableVertexAttribArray(0));
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

		offset += element.count * VertexBufferElement::getTypeSize(element.type);
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(vaoID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}


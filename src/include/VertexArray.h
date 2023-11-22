#pragma once

class VertexBufferLayout;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBufferLayout& layout);

	void Bind()const;
	void Unbind()const;
private:
	unsigned int vaoID;
};


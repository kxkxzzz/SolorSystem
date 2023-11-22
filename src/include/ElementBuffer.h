#pragma once

class ElementBuffer {
private:
	unsigned int eboID;
	unsigned int eboCnt;
public:
	ElementBuffer(const unsigned int* data, unsigned int count);
	~ElementBuffer();

	//void Bind() const;
	void Unbind() const;

	unsigned int getCount() const { return eboCnt; }
};

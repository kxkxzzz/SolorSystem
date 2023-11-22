#pragma once

#include <cmath>
#include <vector>

class Planet {
public:
	Planet();
	Planet(const int precision);	// precision���������ݺỮ�ֵ���������Խ��Խ���徫ϸ
	~Planet();

	int getNumIndices()const { return numIndices; }
	void setNumIndices(const int prec) { numIndices = prec * prec * 6; }
	std::vector<unsigned int> getIndices()const { return indices; }
	std::vector<float> getVertices()const { return vertices; }
private:
	float toRadians(float degrees);
	void init(const int prec);
private:
	int numIndices;
	std::vector<unsigned int> indices;
	std::vector<float> vertices;
};


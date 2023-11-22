#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Planet.h"
#include "ConstParameters.h"
#include "Camera.h"
#include "Texture.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 35.0f));
float lastX = WindowWidth / 2.0f;
float lastY = WindowHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastTime = 0.0f;

void Display(int numIndices) {
	// 剔除背面，不需要对背面进行渲染
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Solar System", nullptr, nullptr);

	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 隐藏光标
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//创建一个球体，生成他的顶点数组并配置好对应的VAO，VBO，EBO
	Planet planet(100);
	std::vector<float> planetVertices = planet.getVertices();
	std::vector<unsigned int> planetIndices = planet.getIndices();

	VertexArray VAO;
	VertexBuffer VBO(&planetVertices[0], planetVertices.size() * sizeof(float));

	VertexBufferLayout layout;
	// 颜色为 3 * float
	layout.Push<float>(3);
	// 纹理坐标为 2 * float
	layout.Push<float>(2);
	VAO.AddBuffer(layout);

	ElementBuffer EBO(&planetIndices[0], planetIndices.size());

	//启用深度测试
	glEnable(GL_DEPTH_TEST);

	Shader shader("shader/shader.vs", "shader/shader.fs");
	shader.Bind();
	shader.setInt("myTexture", 0);

	Texture textureSun("texture/Sun.jpg");
	Texture textureEarth("texture/Earth.jpg");
	Texture textureVenus("texture/Venus.jpg");
	Texture textureJupiter("texture/Jupiter.jpg");
	Texture textureMars("texture/Mars.jpg");

	VBO.Unbind();
	VAO.Unbind();
	EBO.Unbind();
	shader.Unbind();

	//Renderer render;

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//首先处理输入
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();

		// 透视投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) WindowWidth / (float) WindowHeight, 0.1f, 100.0f);

		VAO.Bind();
		{
			// 太阳
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
			// 旋转球体以倾斜
			model = glm::rotate(glm::mat4(1.0f), -(float) glfwGetTime() / 5, glm::vec3(0.0f, 1.0f, 0.0f));
			// 缩放这个球体
			model = scale(model, glm::vec3(SunScale, SunScale, SunScale));
			glm::mat4 mvp = projection * view * model;

			textureSun.Bind();
			shader.Bind();
			shader.setMat4("MVP", mvp);

			Display(planet.getNumIndices());
		}

		{
			// 地球
			glm::mat4 model = glm::mat4(1.0f);
			// 公转
			model = glm::rotate(model, (float) glfwGetTime() / 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			// 先旋转，再根据相对位置做唯一位移
			model = glm::translate(model, glm::vec3(SunEarthDistance, .0f, .0f));
			// 抵消公转对自身倾斜方向的影响，在公转的基础上继续绕 y 轴旋转即可
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -glm::radians(ErothAxialAngle), glm::vec3(0.0, 0.0, 1.0));
			// 自转
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 mvp = projection * view * model;

			textureEarth.Bind();
			shader.Bind();
			shader.setMat4("MVP", mvp);

			Display(planet.getNumIndices());
		}

		{
			// 金星 Venus
			glm::mat4 model = glm::mat4(1.0f);
			// 公转
			model = glm::rotate(model, (float) glfwGetTime() / 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
			// 先旋转，再根据相对位置做唯一位移
			model = glm::translate(model, glm::vec3(SunVenusDistance, .0f, .0f));
			// 抵消公转对自身倾斜方向的影响，在公转的基础上继续绕 y 轴旋转即可
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -glm::radians(ErothAxialAngle), glm::vec3(0.0, 0.0, 1.0));
			// 自转
			model = glm::rotate(model, -(float) glfwGetTime() / 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 mvp = projection * view * model;

			textureVenus.Bind();
			shader.Bind();
			shader.setMat4("MVP", mvp);

			Display(planet.getNumIndices());
		}

		{
			// 木星 Jupiter
			glm::mat4 model = glm::mat4(1.0f);
			model = scale(model, glm::vec3(JupiterScale, JupiterScale, JupiterScale));
			// 公转
			model = glm::rotate(model, (float) glfwGetTime() / 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			// 先旋转，再根据相对位置做唯一位移
			model = glm::translate(model, glm::vec3(SunJupiterDistance, .0f, .0f));
			// 抵消公转对自身倾斜方向的影响，在公转的基础上继续绕 y 轴旋转即可
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -glm::radians(ErothAxialAngle), glm::vec3(0.0, 0.0, 1.0));
			// 自转
			model = glm::rotate(model, -(float) glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 mvp = projection * view * model;

			textureJupiter.Bind();
			shader.Bind();
			shader.setMat4("MVP", mvp);

			Display(planet.getNumIndices());
		}

		{
			// 火星 Mars
			glm::mat4 model = glm::mat4(1.0f);
			model = scale(model, glm::vec3(MarsScale, MarsScale, MarsScale));
			// 公转
			model = glm::rotate(model, (float) glfwGetTime() / 2.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			// 先旋转，再根据相对位置做唯一位移
			model = glm::translate(model, glm::vec3(SunMarsDistance, .0f, .0f));
			// 抵消公转对自身倾斜方向的影响，在公转的基础上继续绕 y 轴旋转即可
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -glm::radians(ErothAxialAngle), glm::vec3(0.0, 0.0, 1.0));
			// 自转
			model = glm::rotate(model, -(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 mvp = projection * view * model;

			textureMars.Bind();
			shader.Bind();
			shader.setMat4("MVP", mvp);

			Display(planet.getNumIndices());
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
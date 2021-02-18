// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <random>

// Include Project Headers
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Data.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

GLFWwindow* window;

// camera
glm::vec3 cameraPos = glm::vec3(260.0f, 100.0f, -40.0f);
glm::vec3 cameraFront = glm::vec3(-0.89f, -0.23f, 0.38f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 SpherePos = glm::vec3(50.0f, 50.0f, 50.0f);
glm::vec4 SphereColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 CubeColor;
glm::vec4 SphereData;
glm::vec3 objectVelocity = glm::vec3(0.05f, 0.05f, 0.05f);

// Light 
glm::vec3 lightPos = glm::vec3(300.0f, 80.0f, 0.0f);
glm::vec3 viewPos;
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

bool firstMouse = true;
int With_Texture = 0;
int SPH_radius = 15;
float yaw = 157.0f;	// yaw and pitched ara initiallized so the camera starts at correct location
float pitch = -13.0f;
float lastX = 600.0f / 2;
float lastY = 600.0f / 2;
float fov = 45.0f;

GLfloat UV_data_SPH[72 * 4 * 4 * 4 / 3 * 2];
GLfloat Normals_data_SPH[72 * 4 * 4 * 4];
GLfloat Normals_data_SC[108];
GLfloat Vertices_data_SPH[72 * 4 * 4 * 4];

GLfloat normals_data_sphere[72 * 4 * 4 * 4];
GLfloat vertices_data_sphere[72 * 4 * 4 * 4];

VertexBufferLayout layout;
std::vector<VertexArray*> Objects;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void createCube(glm::vec4 color, int d, glm::vec3 direction) {
	GLfloat normals_data_cube[108];
	GLfloat vertices_data_cube[108];

	VertexArray* va = new VertexArray{};
	va->SetColor(color);
	va->SetDirection(direction);
	va->SetD(d);
	va->SetType(1);
	va->init();

	for (int i = 0; i < 108; i++) vertices_data_cube[i] = data_cube[i] * d;

	NormalsCreate(vertices_data_cube, 108, normals_data_cube, glm::vec3(0, 0, 0));

	VertexBuffer vertexbuffer(vertices_data_cube, sizeof(vertices_data_cube));
	VertexBuffer uvbuffer(uv_data_cube, sizeof(uv_data_cube));
	VertexBuffer normalsbuffer(normals_data_cube, sizeof(normals_data_cube));

	va->AddBuffer(vertexbuffer, layout, 0);
	va->AddBuffer(uvbuffer, layout, 1);
	va->AddBuffer(normalsbuffer, layout, 2);

	Objects.push_back(va);
	
	va->Unbind();
	vertexbuffer.Unbind();
	normalsbuffer.Unbind();
}

void createSphere(glm::vec4 color, int d, glm::vec3 direction) {

	VertexArray* va = new VertexArray{};
	va->SetColor(color);
	va->SetDirection(direction);
	va->SetD(d);
	va->SetType(2);
	va->init();

	normalizeSphere(data_SPH, 72 * 4 * 4 * 4, vertices_data_sphere, d);

	VertexBuffer vertexbuffer(vertices_data_sphere, sizeof(vertices_data_sphere));
	VertexBuffer uvbuffer(uv_data_cube, sizeof(uv_data_cube));
	VertexBuffer normalsbuffer(Normals_data_SPH, sizeof(Normals_data_SPH));

	va->AddBuffer(vertexbuffer, layout, 0);
	va->AddBuffer(uvbuffer, layout, 1);
	va->AddBuffer(normalsbuffer, layout, 2);

	Objects.push_back(va);

	va->Unbind();
	vertexbuffer.Unbind();
	normalsbuffer.Unbind();
}

void randomObject() {
	std::cout << " Created Object(n." << Objects.size()-1 << "): ";

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.1, 0.9);
	std::uniform_int_distribution<int> dist0(1, 10);
	std::uniform_int_distribution<int> dist1(1, 3);

	glm::vec3 direction = glm::vec3(dist(mt), dist(mt), dist(mt));
	glm::vec4 color = glm::vec4(dist(mt), dist(mt), dist(mt), 1.0f);

	int d = dist0(mt);
	
	int type = dist1(mt);

	switch (type)
	{
	case 1:
		std::cout << "Cube " ;
		createCube(color,d, direction);
		break;
	case 2:
		createSphere(color, d, direction);
		std::cout << "Sphere " ;
		break;
	case 3:
		std::cout << "Cylinder " ; 
		break;
	}

	std::cout << "with d = " << d << std::endl;
	std::cout << "Direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
	std::cout << "Color: " << color.x << " " << color.y << " " << color.z << "\n" << std::endl;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) return; //only handle press events

	if (key == GLFW_KEY_T) { 
		std::cout << " Texture Toggled " << std::endl;
		if (With_Texture == 1) {
			With_Texture = 0;
		}
		else if (With_Texture == 0)
		{
			With_Texture = 1;
		}
	}

	if (key == GLFW_KEY_SPACE) {
		randomObject();
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float cameraSpeed = deltaTime * 150;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		SpherePos.z -= 0.5 * cameraSpeed;
		if (SpherePos.z < SPH_radius)
			SpherePos.z = SPH_radius;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		SpherePos.z += 0.5 * cameraSpeed;
		if (SpherePos.z > 100 - SPH_radius)
			SpherePos.z = 100 - SPH_radius;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		SpherePos.y -= 0.5 * cameraSpeed;
		if (SpherePos.y < SPH_radius)
			SpherePos.y = SPH_radius;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		SpherePos.y += 0.5 * cameraSpeed;
		if (SpherePos.y > 100 - SPH_radius)
			SpherePos.y = 100 - SPH_radius;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		SpherePos.x -= 0.5 * cameraSpeed;
		if (SpherePos.x < SPH_radius)
			SpherePos.x = SPH_radius;
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		SpherePos.x += 0.5 * cameraSpeed;
		if (SpherePos.x > 100 - SPH_radius)
			SpherePos.x = 100 - SPH_radius;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		objectVelocity += cameraSpeed / 100;
		if (objectVelocity.x > 1.2f)
			objectVelocity = glm::vec3(1.2f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		objectVelocity -= cameraSpeed / 100;
		if (objectVelocity.x < 0.0f)
			objectVelocity = glm::vec3(0.0f);
	}
}

void randomCubeColor() {
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0, 1.0);

	CubeColor.r = dist(mt);
	CubeColor.g = dist(mt);
	CubeColor.b = dist(mt);
	CubeColor.a = 0.3f;
}

int main(void)
{
	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(600, 600, "Collisions", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glfwSwapInterval(1); // V-sync

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Keyboard and Mouse 
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	randomCubeColor();
	{
		// Create Shader
		Shader shader("resource files/shaders/VertexShader.vs", "resource files/shaders/FragmentShader.fs");
		shader.Bind();

		glm::mat4 Projection = glm::perspective(glm::radians(fov), (float)600 / (float)600, 0.1f, 400.0f);
		shader.SetUniformMatrix4f("u_P", Projection);

		// Create Texture
		Texture SPH_texture("resource files/textures/sphere_texture.jpg");

		glm::mat4 View;
		glm::mat4 Model;
		glm::mat4 Scale;

		// Create Vertex Array 
		VertexArray va_cube;
		va_cube.SetColor(CubeColor);

		VertexArray va_sphere;
		va_sphere.SetColor(SphereColor);
	
		layout.Push<GLfloat>(3); // LayoutID 0
		layout.Push<GLfloat>(2); // LayoutID 1
		layout.Push<GLfloat>(3); // LayoutID 2
		
		Objects.push_back(&va_sphere);
		Objects.push_back(&va_cube);
								 
		/*
		 
		int length = sizeof(data_doublePyramid) / sizeof(data_doublePyramid[0]);
		
		GLfloat data_SPH_diam[72 * 4];
		diamerizo(data_doublePyramid, length, data_SPH_diam);
		
		GLfloat data_SPH_diam2[72 * 4 * 4];
		diamerizo(data_SPH_diam, length * 4, data_SPH_diam2);
		
		GLfloat data_SPH[72 * 4 * 4 * 4];
		diamerizo(data_SPH_diam2, length * 16, data_SPH);
		
		*/

		UVcreateSphere(data_SPH, 72 * 4 * 4 * 4, UV_data_SPH);
		NormalsCreate(data_SPH, 72 * 4 * 4 * 4, Normals_data_SPH, glm::vec3 (0,0,0));

		normalizeSphere(data_SPH, 72 * 4 * 4 * 4, Vertices_data_SPH, SPH_radius);
		
		NormalsCreate(data_SC, 108, Normals_data_SC, glm::vec3(50, 50, 50));

		// Create Vertex Buffers for SPHERE
		VertexBuffer vertexbuffer_sphere(Vertices_data_SPH, sizeof(Vertices_data_SPH));
		VertexBuffer uvbuffer_sphere(UV_data_SPH, sizeof(UV_data_SPH));
		VertexBuffer normalsbuffer_sphere(Normals_data_SPH, sizeof(Normals_data_SPH));

		// Create Vertex Buffers for CUBE
		VertexBuffer vertexbuffer_cube(data_SC, sizeof(data_SC));
		VertexBuffer uvbuffer_cube(uv_data_cube, sizeof(uv_data_cube));
		VertexBuffer normalsbuffer_cube(Normals_data_SC, sizeof(Normals_data_SC));
		
		va_sphere.AddBuffer(vertexbuffer_sphere, layout, 0);
		va_sphere.AddBuffer(uvbuffer_sphere, layout, 1);
		va_sphere.AddBuffer(normalsbuffer_sphere, layout, 2);

		va_cube.AddBuffer(vertexbuffer_cube, layout, 0);
		va_cube.AddBuffer(uvbuffer_cube, layout, 1);
		va_cube.AddBuffer(normalsbuffer_cube, layout, 2);
		
		shader.Unbind();

		va_sphere.Unbind();
		vertexbuffer_sphere.Unbind(); 
		uvbuffer_sphere.Unbind();
		normalsbuffer_sphere.Unbind();

		va_cube.Unbind();
		vertexbuffer_cube.Unbind();
		uvbuffer_cube.Unbind();
		normalsbuffer_cube.Unbind();

		while (!glfwWindowShouldClose(window))
		{
			
			// per-frame time logic
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// keyboard input
			processInput(window);

			// Clear the screen.
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			shader.Bind();
			SPH_texture.Bind();

			View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			Model = glm::mat4(1.0f);
			shader.SetUniformMatrix4f("u_V", View);
			//Scale = glm::scale(glm::vec3(50.0f, 50.0f, 50.0f));
			
			shader.SetUniform3f("u_lightColor", lightColor.r, lightColor.g, lightColor.b);			
			shader.SetUniform3f("u_viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			shader.SetUniform3f("u_lightPos", cameraPos.x, cameraPos.y, cameraPos.z);

			SphereData = glm::vec4(SpherePos.x, SpherePos.y, SpherePos.z, SPH_radius);
			
			// DRAW SPHERE_SPH
			Objects[0]->SetModel(glm::translate(Model, SpherePos));
			Objects[0]->SetTex(With_Texture);
			Objects[0]->Draw(shader);

			for (int i = 0; i < Objects.size(); i++) {
 
				if ((i == 0) || (i == 1)) continue;
				
				Objects[i]->Draw(shader);
				Objects[i]->Update(objectVelocity, SphereData, Objects);

			}

			// DRAW CUBE_SC		
			Objects[1]->Draw(shader);

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} 
	}
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
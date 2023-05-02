#pragma once

#include <iostream>
#include <GLFW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
typedef
struct shadesrc {

	std::string VertexSource;
	std::string FragmentSource;

}SHADERSRC;

#define FULLSCREEN 1
#define WINDOWED 0

class glWindow {

	GLFWwindow* window;
	

public:
	GLFWwindow* makeWindowW(const char* NameOfTheWIndow,int Height,int Width, bool isFULLSCREEN, bool Vsync);
	unsigned int GLvertexIndexBuffers(void* a, int noOfBuffersToGenerate, unsigned int Type, int SizeOf);
	void pollEvents();
	unsigned int CreateShader(const char* filepath);
	~glWindow();
};
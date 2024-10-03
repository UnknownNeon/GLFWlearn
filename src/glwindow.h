#pragma once


//Strictly 2d 

#include <iostream>
#include <GLFW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "obj_loader.h"

constexpr int WIDTH = 1366;
constexpr int HEIGHT = 768;

typedef struct shadesrc {
	std::string VertexSource;
	std::string FragmentSource;
	
}SHADERSRC;

struct mvp {

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


	void scale_model(float x = 1, float y = 1, float z = 1) {
		this->model = glm::scale(model, glm::vec3(x, y, z));
	}

	void rotate_model(float angle,float x, float y, float z) {
		this->model = glm::rotate(this->model, angle, glm::vec3(x, y, z));
	}

	void translate_model(float x, float y, float z) {
		this->model = glm::translate(model, glm::vec3(x, y, z));
	}
	void translate_view(float x, float y, float z) {
		this->view = glm::translate(view, glm::vec3(x, y, z));
	}

	void prespective_mode(float fov, float near = -1.f, float far = 100.0f) {
		this->projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, near, far);
	}

	void orthographic_mode(float left, float right ,float bottom ,float top ,float near = 0.1 ,float far = 100) {
		this->projection = glm::ortho(left, right, bottom, top , near , far);
	}

	void config(unsigned int shader_program_id) {
		int modelLoc = glGetUniformLocation(shader_program_id, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));


		int viewlLoc = glGetUniformLocation(shader_program_id, "view");
		glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, glm::value_ptr(this->view));

		int projlLoc = glGetUniformLocation(shader_program_id, "projection");
		glUniformMatrix4fv(projlLoc, 1, GL_FALSE, glm::value_ptr(this->projection));

	}
};

class glWindow {


	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	mvp matrices;
	
	//Member Variables 

	GLFWwindow* instance_pointer;
	unsigned int shader_program_id;
	unsigned int vao;
	unsigned int vcount;
	unsigned int DynamicVBO;			//Not Yet Implemented 

	//Objects Section
	std::vector<std::vector<Vertex>> object_manager;
	std::vector<Vertex> render_objects;

	void add_to_scene(std::vector<Vertex> v);
	void translate_object(float x, float y, float z ,int item_number);
	void bake();


	//ObjLoader 
	object_loader loader{};

	//Member Functions 
	void record_vao();
	void end_record_vao();

	void init(const char* NameOfTheWIndow,int Height,int Width, bool isFULLSCREEN, bool Vsync);
	unsigned int create_buffers(void* a, unsigned int Type, int SizeOf);

	unsigned int create_dynamic_buffer(unsigned int Type, unsigned int elements_in_array);
	void push_dbuffer(unsigned int elements_in_array);
	void translate_object_and_add(float x, float y, float z, std::vector<Vertex> v);
	void pollEvents();
	unsigned int create_shader(const char* filepath);
	void init();

	void loop();

public :

	glWindow();
	~glWindow();
};
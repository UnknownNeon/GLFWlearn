#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 position{};
	glm::vec3 color{};
	glm::vec2 texture{};
};

struct v_indices {
	std::vector<unsigned int> vertex_indices;
};

class object_loader {

public:
	std::string destination;
	

	std::vector<unsigned int> vertex_indices;
	std::vector<Vertex> Object;

	std::vector<v_indices> v_i_data;

	void loadObject(std::string& path);
};
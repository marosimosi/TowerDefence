#include "Floor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>


using namespace glm;
using namespace std;


Floor::Floor() {
	vector<vec3> floorVertices = {
		vec3(-38.0f, 0.0f, -28.0f),
		vec3(-38.0f, 0.0f,  28.0f),
		vec3(38.0f, 0.0f,  28.0f),
		vec3(38.0f, 0.0f,  28.0f),
		vec3(38.0f, 0.0f,  -28.0f),
		vec3(-38.0f, 0.0f,  -28.0f)

	};

	// plane normals
	vector<vec3> floorNormals = {
		vec3(0.0, 0.1, 0.0),
		vec3(0.0, 0.1, 0.0),
		vec3(0.0, 0.1, 0.0),
		vec3(0.0, 0.1, 0.0),
		vec3(0.0, 0.1, 0.0),
		vec3(0.0, 0.1, 0.0)
	};

	// plane uvs
	vector<vec2> floorUVs = {
		vec2(0.0f, 0.0f),
		vec2(0.0f, 5.0f),
		vec2(5.0f, 5.0f),
		vec2(5.0f, 5.0f),
		vec2(5.0f, 0.0f),
		vec2(0.0f, 0.0f)
	};

	floor = new Drawable(floorVertices, floorUVs, floorNormals);
	diffuseTexture = loadSOIL("grounddif.jpg");
	specularTexture = loadSOIL("groundspec.jpg");
}

Floor::~Floor() {
    delete floor;
}

void Floor::draw(unsigned int drawable) {
    floor->bind();
    floor->draw();
}

Mountain::Mountain() {
	mountain = new Drawable("mountain.obj");
	diffuseTexture = loadSOIL("mountaindif.jpg");
	specularTexture = loadSOIL("groundspec.jpg");
}

Mountain::~Mountain() {
	delete mountain;
}

void Mountain::draw(unsigned int drawable) {
	mountain->bind();
	mountain->draw();
}


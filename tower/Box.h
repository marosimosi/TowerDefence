#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>
class Box{
public:
	glm::vec3 min;
	glm::vec3 max;

	Box(glm::vec3 min, glm::vec3 max);

	glm::vec3 calculateCenter();
	glm::vec3 calculateDimensions();

	bool containsPoint(glm::vec3 point);
	bool collides(Box* b);

};

#endif
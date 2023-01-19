#include "Box.h"
#include <glm/glm.hpp>

Box::Box(glm::vec3 min, glm::vec3 max){}

glm::vec3 Box::calculateCenter() {
	return ((min + max) / 2.0f);
}

glm::vec3 Box::calculateDimensions() {
	return (max - min);
}

bool Box::containsPoint(glm::vec3 point) {
	return (point.x>=min.x) && (point.x<=max.x) &&
		(point.y >= min.y) && (point.y <= max.y)&&
		(point.z >= min.z) && (point.z <= max.z);

}

bool Box::collides(Box* b) {
	glm::vec3 rad1 = calculateDimensions() / 2.0f;
	glm::vec3 rad2 = b->calculateDimensions() / 2.0f;
	glm::vec3 center1 = calculateCenter();
	glm::vec3 center2 = b->calculateCenter();

	glm::vec3 distance = abs(center1 - center2);

	for (int i = 0; i < 3; i++) {
		if (distance[i] > rad1[i] + rad2[i]) {
			return false;
		}
	}
	return true;
}
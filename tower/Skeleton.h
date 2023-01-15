#ifndef SKELETON_H
#define SKELETON_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Skeleton {
public:
	Drawable* skeleton;
	float hp, speed, damage, load_time;

	Skeleton();
	~Skeleton();
	void draw(unsigned int drawable = 0);
};

#endif


#pragma once

#include "cinder/gl/gl.h"

#include "OscController.h"

struct World {
	ci::ivec2 windowSize;
	std::shared_ptr<OscController> oscController;
	float dt;
	float time;
};

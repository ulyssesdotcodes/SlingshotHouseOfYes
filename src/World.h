#pragma once

#include "cinder/gl/gl.h"

#include "AudioSource.h"
#include "OscController.h"

struct World {
	ci::vec2 windowSize;
	std::shared_ptr<OscController> oscController;
	float dt;
	float time;

	std::shared_ptr<AudioSource> audioSource;
};

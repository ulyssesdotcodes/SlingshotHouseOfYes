#pragma once

#include "World.h"

class Cue {
public:
	virtual void update(const World& world) = 0;
	virtual void draw(const World& world) = 0;

	virtual void transitionTo(const World& world);
	virtual void transitionFrom(const World& world);
};
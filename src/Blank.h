#pragma once

#include "Cue.h"

class Blank : public Cue {
public:
	void update(const World& world) override;
	void draw(const World& world) override;
};
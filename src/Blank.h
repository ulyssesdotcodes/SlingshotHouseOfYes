#pragma once

#include "Cue.h"

class Blank : public Cue {
public:
	void update() override;
	void draw() override;
};
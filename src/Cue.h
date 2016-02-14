#pragma once

class Cue {
public:
	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void transitionTo();
	virtual void transitionFrom();
};
#include "Blank.h"

#include "cinder/gl/gl.h"

using namespace ci;

void Blank::update(const World& world)
{
}

void Blank::draw(const World& world)
{
	gl::clear(Color(0, 0, 0));
}

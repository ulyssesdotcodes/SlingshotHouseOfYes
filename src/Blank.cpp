#include "Blank.h"

#include "cinder/gl/gl.h"

using namespace ci;

void Blank::update()
{
}

void Blank::draw()
{
	gl::clear(Color(0, 0, 0));
}

#pragma once

#include "cinder/gl/gl.h"

class PingPongFBO3D {
public:
	PingPongFBO3D();
	PingPongFBO3D(ci::gl::Fbo::Format fmt, ci::ivec3 size, int buffers);
	ci::gl::Texture3dRef getTexture();
	void render(ci::gl::GlslProgRef shader);
	ci::Area getBounds();
	ci::vec3 getSize();

private:
	std::vector<ci::gl::FboRef> mFBOs;
	std::vector<ci::gl::Texture3dRef> mTextures;
	int mIteration;
	ci::vec3 mSize;

	void drawSolidRect(ci::gl::GlslProgRef prog);
};

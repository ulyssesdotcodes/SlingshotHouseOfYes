#pragma once

#include "cinder/gl/gl.h"

class PingPongFBO3D {
public:
	PingPongFBO3D();
	PingPongFBO3D(ci::gl::Fbo::Format fmt, ci::ivec3 size, int buffers);
	ci::gl::Texture3dRef getTexture();
	std::vector<ci::gl::Texture3dRef> getTextures();
	void render(ci::gl::GlslProgRef shader);

private:
	std::vector<ci::gl::FboRef> mFBOs;
	std::vector<ci::gl::Texture3dRef> mTextures;
	int mIteration;

	Surface createSolid(int width, int height);
};

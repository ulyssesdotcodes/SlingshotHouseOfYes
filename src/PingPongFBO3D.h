#pragma once

#include "cinder/gl/gl.h"

class PingPongFBO3D {
public:
	PingPongFBO3D();
	PingPongFBO3D(ci::gl::Fbo::Format fmt, ci::ivec3 size, int buffers);
	ci::gl::TextureBaseRef getTexture();
	void render(ci::gl::GlslProgRef shader);
	ci::Area getBounds();

private:
	std::vector<ci::gl::FboRef> mFBOs;
	std::vector<ci::gl::Texture3dRef> mTextures;
	int mIteration;
};

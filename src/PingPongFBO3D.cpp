#include "PingPongFBO3D.h"

using namespace ci;

PingPongFBO3D::PingPongFBO3D()
{
}

PingPongFBO3D::PingPongFBO3D(ci::gl::Fbo::Format fmt, ci::ivec3 size, int buffers)
	: mIteration(0)
{
	for (int i = 0; i < buffers; ++i) {
		gl::Texture3dRef tex = gl::Texture3d::create(size.x, size.y, size.z);
		mTextures.push_back(tex);

		fmt.attachment(GL_COLOR_ATTACHMENT0, tex).disableDepth();
		gl::FboRef fboRef = gl::Fbo::create(size.x, size.y, fmt);
		gl::ScopedFramebuffer fbo(fboRef);
		gl::clear(Color(0, 0, 0));
		mFBOs.push_back(fboRef);
	}
}

ci::gl::TextureBaseRef PingPongFBO3D::getTexture()
{
	return mFBOs.at(mIteration % mFBOs.size())->getTextureBase(GL_COLOR_ATTACHMENT0);
}

Area PingPongFBO3D::getBounds()
{
	return mFBOs.at(mIteration % mFBOs.size())->getTextureBase(GL_COLOR_ATTACHMENT0)->getBounds();
}

void PingPongFBO3D::render(ci::gl::GlslProgRef shader)
{
	gl::FboRef target = mFBOs.at((mIteration + 1) % mFBOs.size());

	gl::ScopedFramebuffer fbo(target);
	gl::clear(Color(0, 0, 0));
	gl::ScopedViewport vp(ivec2(0), target->getSize());
	gl::pushMatrices();
	gl::setMatricesWindow(target->getSize());
	gl::ScopedGlslProg glsl(shader);

	gl::drawSolidRect(target->getBounds());

	gl::popMatrices();

	// Increment mIteration here so that `getTexture()` always points to the latest
	++mIteration;
}
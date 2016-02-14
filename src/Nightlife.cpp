#include "Nightlife.h"

#include "cinder/app/App.h"

using namespace ci;

Nightlife::Nightlife()
{
	gl::Texture2d::Format texFmt;
	texFmt.setInternalFormat(GL_RGBA16F);
	texFmt.setDataType(GL_FLOAT);
	texFmt.setTarget(GL_TEXTURE_2D);
	texFmt.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	texFmt.enableMipmapping(false);
	gl::Fbo::Format fmt;
	fmt.disableDepth()
		.setColorTextureFormat(texFmt);

	gl::GlslProg::Format shaderFmt;
	shaderFmt.vertex(app::loadAsset("Shaders/passthru.vert"))
		.fragment(app::loadAsset("Shaders/fade.frag"));
	mFadeShader = gl::GlslProg::create(shaderFmt);
	mFadeShader->uniform("i_resolution", (vec2) app::getWindowSize());

	mParticleRender = gl::Fbo::create(app::getWindowSize().x, app::getWindowSize().y, fmt);
	mPingPongFBO = PingPongFBO(fmt, ivec2(app::getWindowSize().x, app::getWindowSize().y) , 2);
}

void Nightlife::update()
{
	mParticleSystem.update();

	gl::ScopedFramebuffer fbo(mParticleRender);
	gl::clear(Color(0, 0, 0));
	mParticleSystem.draw();
}

void Nightlife::draw()
{
	updateFBO();

	gl::enableAlphaBlending(true);

	gl::pushMatrices();
	gl::setMatricesWindow(app::getWindowSize());
	gl::draw(mPingPongFBO.getTexture());
	gl::popMatrices();
}

void Nightlife::transitionTo()
{
	mParticleSystem.resetParticleSystem();
}

void Nightlife::updateFBO()
{
	gl::ScopedTextureBind prev(mParticleRender->getColorTexture(), 0);
	mFadeShader->uniform("tex_current", 0);

	gl::ScopedTextureBind current(mPingPongFBO.getTexture(), 1);
	mFadeShader->uniform("tex_prev", 1);

	mPingPongFBO.render(mFadeShader);
}

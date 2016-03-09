#include "Nightlife.h"

#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

using namespace ci;

Nightlife::Nightlife(const World& world)
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

	world.oscController->subscribe("/hoy/cues/nightlife/startx/value", [=](const osc::Message message) {
			mParticleSystem.setStartPositionX(message.getArgFloat(0));
	});

	world.oscController->subscribe("/hoy/cues/nightlife/starty/value", [=](const osc::Message message) {
			mParticleSystem.setStartPositionY(message.getArgFloat(0));
	});

	world.oscController->subscribe("/hoy/cues/nightlife/endx/value", [=](const osc::Message message) {
			mParticleSystem.setEndPositionX(message.getArgFloat(0));
	});
	world.oscController->subscribe("/hoy/cues/nightlife/endy/value", [=](const osc::Message message) {
			mParticleSystem.setEndPositionY(message.getArgFloat(0));
	});

	world.oscController->subscribe("/hoy/cues/nightlife/midx/value", [=](const osc::Message message) {
			mParticleSystem.setMiddlePositionX(message.getArgFloat(0));
	});
	world.oscController->subscribe("/hoy/cues/nightlife/midy/value", [=](const osc::Message message) {
			mParticleSystem.setMiddlePositionY(message.getArgFloat(0));
	});
	world.oscController->subscribe("/hoy/cues/nightlife/reset/value", [=](const osc::Message message) {
		mParticleSystem.resetParticleSystem();
	});
	world.oscController->subscribe("/hoy/cues/nightlife/animate/value", [=](const osc::Message message) {
		mParticleSystem.animate(message.getArgFloat(0) == 1);
	});

	world.oscController->subscribe("/hoy/cues/nightlife/end/value", [=](const osc::Message message) {
		mParticleSystem.end();
	});
}

void Nightlife::update(const World& world)
{
	mParticleSystem.update();

	gl::ScopedFramebuffer fbo(mParticleRender);
	gl::clear(Color(0, 0, 0));
	mParticleSystem.draw();
}

void Nightlife::draw(const World& world)
{
	updateFBO();

	gl::enableDepthWrite();
	gl::enableDepthRead();

	gl::enableAlphaBlending(true);

	gl::pushMatrices();
	gl::setMatricesWindow(app::getWindowSize());
	gl::draw(mPingPongFBO.getTexture());
	gl::popMatrices();
}

void Nightlife::transitionTo(const World& world)
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

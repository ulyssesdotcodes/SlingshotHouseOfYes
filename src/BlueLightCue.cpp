#include "BlueLightCue.h"

using namespace ci;

BlueLightCue::BlueLightCue(const World & world)
	//:mCam( app::getWindowWidth(), app::getWindowHeight(), 90.0f, 0.1f, 20.0f )
	: mDropPos(vec2(0.5, 0.2))
{
	//mCam.lookAt( vec3( 0.0f, 0.0f, 2.0f ), vec3( 0 ) );

	vec2 fluidResolution = vec2(1024, 512);
	mFluid = Fluid(fluidResolution);

	gl::GlslProg::Format updateFormat;
	updateFormat.vertex(app::loadAsset("Shaders/passthru.vert"));
	//updateFormat.geometry(app::loadAsset("Shaders/Fluid/2d/raycast.geom"));
	updateFormat.fragment(app::loadAsset("Shaders/Fluid/2D/blue_light_draw.frag"));
	//mRaycastShader = gl::GlslProg::create(updateFormat);
	//mRaycastShader->uniform("i_resolution", world.windowSize);
	//mRaycastShader->uniform("i_origin", mCam.getEyePoint());
	mSmokeDrawShader = gl::GlslProg::create(updateFormat);
	mSmokeDrawShader->uniform("i_resolution", world.windowSize);

	//updateFormat.vertex(app::loadAsset("Shaders/Fluid/pick.vert"));
	//updateFormat.geometry(app::loadAsset("Shaders/Fluid/pick.geom"));
	updateFormat.fragment(app::loadAsset("Shaders/Fluid/2D/blue_light_forces.frag"));
	mForcesShader = gl::GlslProg::create(updateFormat);
	mForcesShader->uniform("i_resolution", fluidResolution);

	updateFormat.fragment(app::loadAsset("Shaders/Fluid/2D/blue_light_drop.frag"));
	mSmokeDropShader = gl::GlslProg::create(updateFormat);
	mSmokeDropShader->uniform("i_resolution", world.windowSize);
	mSmokeDropShader->uniform("i_dropPos", mDropPos);
	mSmokeDropShader->uniform("i_dropSize", 0.004f);
	mSmokeDropShader->uniform("i_mult", 1.0f);

	gl::Texture2d::Format texFmt;
	texFmt.setInternalFormat(GL_RGBA16F);
	texFmt.setDataType(GL_HALF_FLOAT);
	texFmt.setTarget(GL_TEXTURE_2D);
	texFmt.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	gl::Fbo::Format fmt;
	fmt.disableDepth()
		.setColorTextureFormat(texFmt);
	mSmokeField = PingPongFBO(fmt, world.windowSize, 2);

	world.oscController->subscribe("/hoy/cues/bluelight/intensity/value", [=](const osc::Message message) {
		mSmokeDropShader->uniform("i_mult", message.getArgFloat(0));
	});

	world.oscController->subscribe("/hoy/cues/bluelight/size/value", [=](const osc::Message message) {
		mSmokeDropShader->uniform("i_dropSize", message.getArgFloat(0));
	});

	world.oscController->subscribe("/hoy/cues/bluelight/x/value", [=](const osc::Message message) {
		mDropPos.x = message.getArgFloat(0);
		mSmokeDropShader->uniform("i_dropPos", mDropPos);
	});

	world.oscController->subscribe("/hoy/cues/bluelight/y/value", [=](const osc::Message message) {
		mDropPos.y = message.getArgFloat(0);
		mSmokeDropShader->uniform("i_dropPos", mDropPos);
	});
}

void BlueLightCue::update(const World & world)
{
	mForcesShader->uniform("i_dt", world.dt);
	mForcesShader->uniform("i_time", world.time);
	mFluid.update(world.dt, mForcesShader, mSmokeField.getTexture(), 0.99);

	// Use the fluid to advect the smoke
	mFluid.advect(world.dt, &mSmokeField, 0.995);


	//gl::ScopedTextureBind scopeBlueLightTexDrop(mBlueLightDropTex.getTexture(), 2);
	//mBlueLightDropTexShader->uniform("tex_fire", 2);

	//mBlueLightDropTex.render(mBlueLightDropTexShader);

	gl::ScopedTextureBind scopeSmokeDrop(mSmokeField.getTexture(), 0);
	mSmokeDropShader->uniform("tex_prev", 0);
	mSmokeDropShader->uniform("i_dt", world.dt);
	mSmokeDropShader->uniform("i_time", world.time);
	mSmokeDropShader->uniform("i_volume", world.audioSource->getVolume());
	mSmokeField.render(mSmokeDropShader);
}

void BlueLightCue::draw(const World & world)
{
	gl::ScopedTextureBind smokeTex(mSmokeField.getTexture(), 0);
	//gl::ScopedTextureBind smokeTex(mFluid.getVelocityTexture(), 0);
	//mRaycastShader->uniform("tex_smoke", 0);
	mSmokeDrawShader->uniform("tex", 0);

	gl::ScopedViewport vp(ivec2(0), world.windowSize);
	//gl::ScopedGlslProg glsl(mRaycastShader);
	gl::ScopedGlslProg glsl(mSmokeDrawShader);

	gl::enableDepthRead();
	gl::enableDepthWrite();

	gl::pushMatrices();
	//gl::setMatrices(mCam);
	gl::setMatricesWindow(world.windowSize);

	//mBatch->draw();
	gl::drawSolidRect(Rectf(vec2(0), world.windowSize));

	gl::popMatrices();
}

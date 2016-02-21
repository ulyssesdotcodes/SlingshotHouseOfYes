#include "FireCue.h"

using namespace ci;

FireCue::FireCue(const World & world)
	:mCam( app::getWindowWidth(), app::getWindowHeight(), 90.0f, 0.1f, 20.0f )
{
	mCam.lookAt( vec3( 0.0f, 0.0f, 2.0f ), vec3( 0 ) );

	vec3 fluidResolution = vec3(512, 512, 64);
	mFluid = Fluid3D(fluidResolution);

	gl::GlslProg::Format updateFormat;
	updateFormat.vertex(app::loadAsset("Shaders/passthru.vert"));
	updateFormat.geometry(app::loadAsset("Shaders/Fluid/raycast.geom"));
	updateFormat.fragment(app::loadAsset("Shaders/Fluid/raycast.frag"));
	mRaycastShader = gl::GlslProg::create(updateFormat);
	mRaycastShader->uniform("i_resolution", world.windowSize);
	mRaycastShader->uniform("i_origin", mCam.getEyePoint());

	updateFormat.vertex(app::loadAsset("Shaders/Fluid/pick.vert"));
	updateFormat.geometry(app::loadAsset("Shaders/Fluid/pick.geom"));
	updateFormat.fragment(app::loadAsset("Shaders/Fluid/smoke_drop_forces.frag"));
	mForcesShader = gl::GlslProg::create(updateFormat);
	mForcesShader->uniform("i_resolution", fluidResolution);

	updateFormat.fragment(app::loadAsset("Shaders/Fluid/smoke_drop.frag"));
	mSmokeDropShader = gl::GlslProg::create(updateFormat);
	mSmokeDropShader->uniform("i_resolution", fluidResolution);
	mSmokeDropShader->uniform("i_smokeDropPos", vec2(0.5, 0.5));


	gl::Texture2d::Format texFmt;
	texFmt.setInternalFormat(GL_RGBA32F);
	texFmt.setDataType(GL_FLOAT);
	texFmt.setTarget(GL_TEXTURE_3D);
	texFmt.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	gl::Fbo::Format fmt;
	fmt.disableDepth()
		.setColorTextureFormat(texFmt);
	mSmokeField = PingPongFBO3D(fmt, fluidResolution, 4);

	mBatch = gl::VertBatch::create();
	mBatch->vertex(vec3(0));
}

void FireCue::update(const World & world)
{
	mForcesShader->uniform("i_dt", world.dt);
	mFluid.update(world.dt, mForcesShader, mSmokeField.getTexture());

	gl::ScopedTextureBind scopeSmokeDrop(mSmokeField.getTexture(), 0);
	mSmokeDropShader->uniform("tex_prev", 0);
	mSmokeDropShader->uniform("i_dt", world.dt);
	mSmokeField.render(mSmokeDropShader);

	// Use the fluid to advect the smoke
	mFluid.advect(world.dt, &mSmokeField);
}

void FireCue::draw(const World & world)
{
	gl::ScopedTextureBind smokeTex(mSmokeField.getTexture(), 0);
	mRaycastShader->uniform("tex_smoke", 0);

	gl::ScopedViewport vp(ivec2(0), world.windowSize);
	gl::ScopedGlslProg glsl(mRaycastShader);

	gl::enableDepthRead();
	gl::enableDepthWrite();

	gl::setMatrices(mCam);

	mBatch->draw();
}
#include "FireCue.h"

using namespace ci;

FireCue::FireCue(const World & world)
{
	vec3 fluidResolution = vec3(world.windowSize.x * 0.25, world.windowSize.y * 0.25, 16);
	mFluid = Fluid3D(fluidResolution);

	gl::GlslProg::Format updateFormat;
	updateFormat.vertex(app::loadAsset("Shaders/passthru.vert"));
	updateFormat.geometry(app::loadAsset("Shaders/Fluid/pick.geom"));
	updateFormat.fragment(app::loadAsset("Shaders/Fluid/smoke_drop_forces.frag"));
	mForcesShader = gl::GlslProg::create(updateFormat);
	mForcesShader->uniform("i_resolution", fluidResolution);
}

void FireCue::update(const World & world)
{
	mFluid.update(world.dt, mForcesShader, mSmokeField.getTexture());
}

void FireCue::draw(const World & world)
{
}

void FireCue::updateFBO()
{
}

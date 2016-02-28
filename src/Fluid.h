#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/BufferTexture.h"
#include "cinder/app/App.h"

#include "PingPongFBO.h"

// Holds the state for a fluid simulation. This involves calculating and storing the velocity field. A glsl program is
// provided in `update` to update forces. Other quantities are advected through the advect method.
class Fluid {
public:
	Fluid();
	Fluid(ci::vec2 fluidResolution);

	// Update the main velocity state
	void update(float dt, ci::gl::GlslProgRef forces, ci::gl::TextureRef smoke, float dissipation);

	// Advect a texture based on the current velocity
	void advect(float dt, PingPongFBO* value, float dissipation);

	PingPongFBO mVelocityFBO, mPressureFBO, mDivergenceFBO;

private:
	ci::vec2 mFluidResolution;

	ci::gl::GlslProgRef
		mAdvectShader,
		mAdvectMaccormackShader,
		mDivergenceShader,
		mPressureSolveShader,
		mSubtractPressureShader;

	ci::gl::TextureRef mObstacles;

	void applyForces(ci::gl::GlslProgRef forces, ci::gl::TextureRef smoke);
	void computeDivergence();
	void solvePressure();
	void subtractPressure();

};

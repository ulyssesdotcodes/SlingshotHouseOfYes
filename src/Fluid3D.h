#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/BufferTexture.h"
#include "cinder/app/App.h"

#include "PingPongFBO3D.h"

// Holds the state for a fluid simulation. This involves calculating and storing the velocity field. A glsl program is
// provided in `update` to update forces. Other quantities are advected through the advect method.
class Fluid3D {
public:
	Fluid3D();
	Fluid3D(ci::vec3 fluidResolution);

	// Update the main velocity state
	void update(float dt, ci::gl::GlslProgRef forces, ci::gl::Texture3dRef smoke);

	// Advect a texture based on the current velocity
	void advect(float dt, PingPongFBO3D* value);

	ci::gl::Texture3dRef getVelocityTexture();
	ci::gl::Texture3dRef getPressureTexture();

	PingPongFBO3D mVelocityFBO, mPressureFBO;

private:
	ci::vec3 mFluidResolution;

	ci::gl::GlslProgRef
		mAdvectShader,
		mAdvectMaccormackShader,
		mDivergenceShader,
		mPressureSolveShader,
		mSubtractPressureShader;

	std::vector<ci::vec4> mVerts;


	void applyForces(ci::gl::GlslProgRef forces, ci::gl::Texture3dRef smoke);
	void computeDivergence();
	void solvePressure();
	void subtractPressure();
};

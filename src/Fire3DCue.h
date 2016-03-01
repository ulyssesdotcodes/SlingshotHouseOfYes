#pragma once

#include "cinder/gl/gl.h"

#include "Cue.h"
#include "Fluid3D.h"

class Fire3DCue : public Cue {
public:
	Fire3DCue(const World& world);
	void update(const World& world) override;
	void draw(const World& world) override;

private:
	Fluid3D mFluid;
	PingPongFBO3D mSmokeField;

	ci::gl::GlslProgRef mForcesShader,
		mSmokeDropShader,
		mRaycastShader;

	ci::gl::TextureRef mHoYTex;

	ci::gl::VertBatchRef mBatch;
	ci::CameraPersp	mCam;
};

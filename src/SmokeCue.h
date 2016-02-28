#pragma once

#include "cinder/gl/gl.h"

#include "Cue.h"
#include "Fluid.h"

class SmokeCue : public Cue {
public:
	SmokeCue(const World& world);
	void update(const World& world) override;
	void draw(const World& world) override;

private:
	Fluid mFluid;
	PingPongFBO mSmokeField;

	ci::gl::GlslProgRef mForcesShader,
		mSmokeDropShader,
		mSmokeDrawShader;
		//mRaycastShader;

	ci::gl::TextureRef mHoYTex;

	//ci::gl::VertBatchRef mBatch;
	//ci::CameraPersp	mCam;
};

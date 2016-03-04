#pragma once

#include "cinder/gl/gl.h"

#include "Cue.h"
#include "Fluid.h"

class BlueLightCue : public Cue {
public:
	BlueLightCue(const World& world);
	void update(const World& world) override;
	void draw(const World& world) override;

private:
	Fluid mFluid;
	PingPongFBO mSmokeField;

	ci::gl::GlslProgRef mForcesShader,
		mSmokeDropShader,
		mSmokeDrawShader,
		mBlueLightDropTexShader;
		//mRaycastShader;

	ci::vec2 mDropPos;

	//ci::gl::VertBatchRef mBatch;
	//ci::CameraPersp	mCam;
};

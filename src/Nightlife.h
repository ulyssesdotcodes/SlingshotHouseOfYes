#pragma once

#include "cinder/gl/gl.h"

#include "Cue.h"
#include "ParticleSystem.h"
#include "PingPongFBO.h"

class Nightlife : public Cue {
public:
	Nightlife();
	void update() override;
	void draw() override;

	void transitionTo() override;

private:
	ParticleSystem mParticleSystem;

	ci::gl::FboRef mParticleRender;
	PingPongFBO mPingPongFBO;

	ci::gl::GlslProgRef mFadeShader;

	void updateFBO(); 
};
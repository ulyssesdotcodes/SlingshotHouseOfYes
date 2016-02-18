#pragma once

#include "cinder/gl/gl.h"

#include "Cue.h"
#include "ParticleSystem.h"
#include "PingPongFBO.h"

class Nightlife : public Cue {
public:
	Nightlife(const World& world);
	void update(const World& world) override;
	void draw(const World& world) override;

	void transitionTo(const World& world) override;

private:
	ParticleSystem mParticleSystem;

	ci::gl::FboRef mParticleRender;
	PingPongFBO mPingPongFBO;

	ci::gl::GlslProgRef mFadeShader;

	void updateFBO(); 
};
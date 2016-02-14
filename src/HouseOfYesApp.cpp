#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ParticleSystem.h"
#include "PingPongFBO.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HouseOfYesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void updateFBO();
	void draw() override;

private:
	ParticleSystem mParticleSystem;

	gl::FboRef mParticleRender;
	PingPongFBO mPingPongFBO;

	gl::GlslProgRef mFadeShader;

	gl::TextureRef mTexture;

};

void HouseOfYesApp::setup()
{
	gl::Texture2d::Format texFmt;
	texFmt.setInternalFormat(GL_RGBA16F);
	texFmt.setDataType(GL_FLOAT);
	texFmt.setTarget(GL_TEXTURE_2D);
	texFmt.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	texFmt.enableMipmapping(false);
	gl::Fbo::Format fmt;
	fmt.disableDepth()
		.setColorTextureFormat(texFmt);

	gl::GlslProg::Format shaderFmt;
	shaderFmt.vertex(app::loadAsset("Shaders/passthru.vert"))
		.fragment(app::loadAsset("Shaders/fade.frag"));
	mFadeShader = gl::GlslProg::create(shaderFmt);
	mFadeShader->uniform("i_resolution", (vec2) getWindowSize());

	mParticleRender = gl::Fbo::create(getWindowSize().x, getWindowSize().y, fmt);
	mPingPongFBO = PingPongFBO(fmt, ivec2(getWindowSize().x, getWindowSize().y) , 2);

	// HoY image
	mTexture = gl::Texture::create(loadImage(loadAsset("Images/HOYSplineMask.jpg")));
}

void HouseOfYesApp::mouseDown( MouseEvent event )
{
}

void HouseOfYesApp::update()
{
	mParticleSystem.update();

	gl::ScopedFramebuffer fbo(mParticleRender);
	gl::clear(Color(0, 0, 0));
	mParticleSystem.draw();
}

void HouseOfYesApp::updateFBO()
{
	gl::ScopedTextureBind prev(mParticleRender->getColorTexture(), 0);
	mFadeShader->uniform("tex_current", 0);

	gl::ScopedTextureBind current(mPingPongFBO.getTexture(), 1);
	mFadeShader->uniform("tex_prev", 1);

	mPingPongFBO.render(mFadeShader);
}

void HouseOfYesApp::draw()
{
	updateFBO();

	gl::enableAlphaBlending(true);

	gl::clear(Color(0, 0, 0));

	// First draw HoY Spline

	gl::pushMatrices();
	gl::setMatricesWindow(getWindowSize());
	Rectf destRect = Rectf( mTexture->getBounds() ).getCenteredFit( getWindowBounds(), true );
	gl::draw( mTexture, destRect );
	gl::popMatrices();

	// Then draw FBO
	gl::pushMatrices();
	gl::setMatricesWindow(getWindowSize());
	gl::draw(mPingPongFBO.getTexture());
	gl::popMatrices();
}

CINDER_APP( HouseOfYesApp, RendererGl,
	[&]( App::Settings *settings )
{
	FullScreenOptions options;
	std::vector<DisplayRef> displays = Display::getDisplays();
	settings->setFullScreen(true, options);	
	if (displays.size() > 1) {
		settings->setDisplay(displays[1]);
	}
	settings->setFrameRate(60.0f);
} )

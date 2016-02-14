#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Blank.h"
#include "Nightlife.h"
#include "OscController.h"
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
	void draw() override;

private:
	std::shared_ptr<OscController> mOscController;

	std::map<std::string, std::shared_ptr<Cue>> mCues;
	std::shared_ptr<Cue> mCurrentCue;

	gl::TextureRef mTexture;
};

void HouseOfYesApp::setup()
{
	mOscController = std::shared_ptr<OscController>();

	mCues.insert(make_pair("Blank", std::make_shared<Blank>()));
	mCues.insert(make_pair("Nightlife", std::make_shared<Nightlife>()));
	mCurrentCue = mCues["Nightlife"];

	// HoY image
	mTexture = gl::Texture::create(loadImage(loadAsset("Images/HOYSplineMask.png")));
}

void HouseOfYesApp::mouseDown( MouseEvent event )
{
}

void HouseOfYesApp::update()
{
	mCurrentCue->update();
}

void HouseOfYesApp::draw()
{
	gl::clear(Color(0, 0, 0));

	gl::enableAlphaBlending(true);

	// Then draw our cue
	mCurrentCue->draw();

	// First draw HoY Spline
	gl::pushMatrices();
	gl::setMatricesWindow(getWindowSize());
	Rectf destRect = Rectf( mTexture->getBounds() ).getCenteredFit( getWindowBounds(), true );
	gl::draw( mTexture, destRect );
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

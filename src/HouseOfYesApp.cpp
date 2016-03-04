#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Blank.h"
#include "BlueLightCue.h"
#include "FireCue.h"
#include "Nightlife.h"
#include "OscController.h"
#include "ParticleSystem.h"
#include "PingPongFBO.h"
#include "SmokeCue.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HouseOfYesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	void setCue(std::string);

private:
	std::shared_ptr<OscController> mOscController;

	std::map<std::string, std::shared_ptr<Cue>> mCues;
	std::shared_ptr<Cue> mCurrentCue;

	gl::TextureRef mTexture;

	World mWorld;
};

void HouseOfYesApp::setup()
{
	mOscController = std::make_shared<OscController>();

	mWorld.oscController = mOscController;
	mWorld.windowSize = app::getWindowSize();
	mWorld.time = 0;
	mWorld.dt = 0;

	mCues.insert(make_pair("Blank", std::make_shared<Blank>()));
	mCues.insert(make_pair("Nightlife", std::make_shared<Nightlife>(mWorld)));
	mCues.insert(make_pair("Fire", std::make_shared<FireCue>(mWorld)));
	mCues.insert(make_pair("Smoke", std::make_shared<SmokeCue>(mWorld)));
	mCues.insert(make_pair("BlueLight", std::make_shared<BlueLightCue>(mWorld)));
	mCurrentCue = mCues["BlueLight"];

	// HoY image
	//mTexture = gl::Texture::create(loadImage(loadAsset("Images/HOYSplineMask.png")));

	mOscController->subscribe("/hoy/program", [=](const osc::Message message) {
		setCue(message.getArgString(0));
	});
}

void HouseOfYesApp::mouseDown( MouseEvent event )
{
}

void HouseOfYesApp::update()
{
	mWorld.dt = getElapsedSeconds() - mWorld.time;
	mWorld.time = getElapsedSeconds();
	mCurrentCue->update(mWorld);
}

void HouseOfYesApp::draw()
{
	if(mCurrentCue == nullptr) {
		return;
	}

	gl::clear(Color(0, 0, 0));

	gl::enableAlphaBlending(true);

	mCurrentCue->draw(mWorld);

	gl::pushMatrices();
	gl::setMatricesWindow(getWindowSize());
	//Rectf destRect = Rectf( mTexture->getBounds() ).getCenteredFit( getWindowBounds(), true );
	gl::drawString("Framerate: " + std::to_string(getAverageFps()), vec2(10));
	//gl::draw( mTexture, destRect );
	gl::popMatrices();
	
}

void HouseOfYesApp::setCue(std::string name)
{
	mCurrentCue->transitionFrom(mWorld);
	mCurrentCue = mCues[name];
	mCurrentCue->transitionTo(mWorld);
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

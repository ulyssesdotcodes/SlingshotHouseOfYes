#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HouseOfYesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HouseOfYesApp::setup()
{
}

void HouseOfYesApp::mouseDown( MouseEvent event )
{
}

void HouseOfYesApp::update()
{
}

void HouseOfYesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HouseOfYesApp, RendererGl )

#include "PingPongFBO3D.h"

using namespace ci;

PingPongFBO3D::PingPongFBO3D()
{
}

PingPongFBO3D::PingPongFBO3D(ci::gl::Fbo::Format fmt, ci::ivec3 size, int buffers)
	: mIteration(0)
{
	mSize = size;
	for (int i = 0; i < buffers; ++i) {
		gl::Texture2d::Format cFmt = fmt.getColorTextureFormat();
		gl::Texture3d::Format fmt3d;
		fmt3d.setInternalFormat(cFmt.getInternalFormat());
		fmt3d.setTarget(GL_TEXTURE_3D);
		fmt3d.setWrap(cFmt.getWrapS(), cFmt.getWrapT(), cFmt.getWrapR());
		fmt3d.setMagFilter(cFmt.getMagFilter());
		fmt3d.setMagFilter(cFmt.getMinFilter());

		gl::Texture3dRef tex = gl::Texture3d::create(size.x, size.y, size.z, fmt3d);
		mTextures.push_back(tex);

		fmt.attachment(GL_COLOR_ATTACHMENT0, tex).disableDepth();
		gl::FboRef fboRef = gl::Fbo::create(size.x, size.y, fmt);
		gl::ScopedFramebuffer fbo(fboRef);
		gl::clear(Color(0, 0, 0));
		mFBOs.push_back(fboRef);
	}
}

ci::gl::Texture3dRef PingPongFBO3D::getTexture()
{
	return mTextures[mIteration % mFBOs.size()];;
}

Area PingPongFBO3D::getBounds()
{
	return mTextures[mIteration % mFBOs.size()]->getBounds();
}

vec3 PingPongFBO3D::getSize()
{
	return mSize;
}

void PingPongFBO3D::render(ci::gl::GlslProgRef shader)
{
	gl::FboRef target = mFBOs.at((mIteration + 1) % mFBOs.size());

	gl::ScopedFramebuffer fbo(target);
	gl::clear(Color(0, 0, 0));
	gl::ScopedViewport vp(ivec2(0), target->getSize());
	//gl::pushMatrices();
	//gl::setMatricesWindow(target->getSize());
	gl::ScopedGlslProg glsl(shader);

	drawSolidRect(shader);

	//gl::popMatrices();

	// Increment mIteration here so that `getTexture()` always points to the latest
	++mIteration;
}

void PingPongFBO3D::drawSolidRect(gl::GlslProgRef prog)
{
	auto ctx = gl::context();

	Rectf r = Rectf(1, -1, -1, 1);

	GLfloat data[8]; // both verts and texCoords
	GLfloat *verts = data;

	verts[0*2+0] = r.getX2(); 
	verts[0*2+1] = r.getY1(); 
	verts[1*2+0] = r.getX1(); 
	verts[1*2+1] = r.getY1(); 
	verts[2*2+0] = r.getX2(); 
	verts[2*2+1] = r.getY2(); 
	verts[3*2+0] = r.getX1(); 
	verts[3*2+1] = r.getY2(); 

	ctx->pushVao();
	ctx->getDefaultVao()->replacementBindBegin();
	gl::VboRef defaultVbo = ctx->getDefaultArrayVbo(sizeof(float) * 8);
	gl::ScopedBuffer bufferBindScp(defaultVbo);
	defaultVbo->bufferSubData(0, sizeof(float) * 8, data);

	int posLoc = prog->getAttribSemanticLocation(geom::Attrib::POSITION);
	if (posLoc >= 0) {
		gl::enableVertexAttribArray(posLoc);
		gl::vertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	ctx->getDefaultVao()->replacementBindEnd();
	ctx->setDefaultShaderVars();
	gl::drawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 32);
	ctx->popVao();
}
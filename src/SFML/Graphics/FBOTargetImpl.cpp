#include <SFML/Graphics/FBOTargetImpl.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/Err.hpp>

namespace sf
{

    sf::RenderWindow* FBOTargetImpl::sContextOwner = NULL;

    FBOTargetImpl::FBOTargetImpl() :mFbo(0), mTex(0){

    }

    FBOTargetImpl::~FBOTargetImpl(){
        // cleanup (will auto kill the texture, but need to delete the fbo)
        if (mFbo != 0){
            glCheck(GLEXT_glDeleteFramebuffers(1, &mFbo));
        }
    }

    // Set the context for FBOs to be created on
    // Must call this before any create(...) calls will work
    void FBOTargetImpl::SetContextOwner(RenderWindow* window){
        sContextOwner = window;
    }

    // Create the FBOTarget
    bool FBOTargetImpl::create(unsigned int width, unsigned int height, bool depthBuffer /* = false */) {
        if (sContextOwner == NULL) return false;

        // TODO: Fallback to sf::RenderTexture if we can't make FBOs

        // Create the objects using the shared context		
        sContextOwner->setActive(true);

        if (!mTexture.create(width, height)){
            err() << "Couldn't create texture";
            return false;
        }
        mTexture.setSmooth(false);
        mTexture.m_fboAttachment = true;

        // get texture id
        mTex = mTexture.getNativeHandle();
        /*GLint tex;
        sf::Texture::bind(&mTexture);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        if (tex >= 0) mTex = tex;
        */

        // make fbo
        glCheck(GLEXT_glGenFramebuffers(1, &mFbo));
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, mFbo));

        //Attach 2D texture to this FBO
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0));

        //Does the GPU support current FBO configuration?
        GLenum status;
        status = glCheck(GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
        if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE) {
            // Badness
            err() << "Couldn't create fbo";
            glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0));
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
            return false;
        }

        // Finally, initialize the parent
        RenderTarget::initialize();

        return true;
    }

    bool FBOTargetImpl::setActive(bool active) {
        bool res = sContextOwner->setActive(true);
        if (active) {
            GLint currentFbo;
            glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &currentFbo));
            if (currentFbo != mFbo){
                glCheck(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFbo));
                glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0));
            }
        }
        else {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        }

        return true;
    }

    void FBOTargetImpl::display() {
        if (setActive(true)) {
            glFlush();
            mTexture.m_pixelsFlipped = true;
        }
    }

    Vector2u FBOTargetImpl::getSize() const {
        // NB: getSize() is called in initialise to setup the default viewport
        return mTexture.getSize();
    };

    const Texture& FBOTargetImpl::getTexture() const {
        return mTexture;
    }

    bool FBOTargetImpl::activate(bool active) {
        return setActive(active);
    }

    void FBOTargetImpl::setSmooth(bool smooth) {
        mTexture.setSmooth(smooth);
    }
}
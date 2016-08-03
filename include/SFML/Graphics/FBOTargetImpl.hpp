#ifndef SFML_FBOTARGETIMPL_HPP
#define SFML_FBOTARGETIMPL_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf {
    // A single-context Framebuffer render target
    // A lighter version of sf::RenderTexture
    // NB: MUST call SetContextOwner before create()
    class SFML_GRAPHICS_API FBOTargetImpl : public sf::RenderTexture {
    public:
        FBOTargetImpl();
        virtual ~FBOTargetImpl();

        // Set the context for FBOs to be created on
        // Must call this before any create(...) calls will work
        static void SetContextOwner(RenderWindow* window);

        // Create the FBOTarget
        virtual bool create(unsigned int width, unsigned int height, bool depthBuffer = false);

        virtual bool setActive(bool active = true);
        virtual void display();
        virtual Vector2u getSize() const;
        virtual const Texture& getTexture() const;

        virtual void setSmooth(bool smooth);

    private:
        virtual bool activate(bool active);

        // Shared context
        static RenderWindow* sContextOwner;

        // Internals
        unsigned int mFbo; // FBO id
        Texture mTexture; // Target texture to draw on
        unsigned int mTex;
    };

} //namespace sf

#endif //SFML_FBOTARGETIMPL_HPP

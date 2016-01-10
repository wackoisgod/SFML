#ifndef SFML_FBOTARGETIMPL_HPP
#define SFML_FBOTARGETIMPL_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf {
    // A single-context Framebuffer render target
    // A lighter version of sf::RenderTexture
    // NB: MUST call SetContextOwner before create()
    class SFML_GRAPHICS_API FBOTargetImpl : public sf::RenderTarget {
    public:
        FBOTargetImpl();
        virtual ~FBOTargetImpl();

        // Set the context for FBOs to be created on
        // Must call this before any create(...) calls will work
        static void SetContextOwner(sf::RenderWindow* window);

        // Create the FBOTarget
        bool create(unsigned int w, unsigned int h);

        bool setActive(bool active = true);
        void display();
        virtual sf::Vector2u getSize() const;
        const sf::Texture& getTexture() const;

        void setSmooth(bool smooth);

    private:
        virtual bool activate(bool active);

        // Shared context
        static sf::RenderWindow* sContextOwner;

        // Internals
        unsigned int mFbo; // FBO id
        sf::Texture mTexture; // Target texture to draw on
        unsigned int mTex;
    };

} //namespace sf

#endif //SFML_FBOTARGETIMPL_HPP

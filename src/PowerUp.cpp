#include "PowerUp.hpp"

PowerUp::PowerUp(float x, float y, Type type) : mType(type) {
    mShape.setPosition(x, y);
    mShape.setRadius(15.f); // Un poco más grande que la pelota
    
    // Color según el tipo
    if (mType == SPEED_BOOST) {
        mShape.setFillColor(sf::Color::Red);   // Rojo = Peligro/Velocidad
    } else {
        mShape.setFillColor(sf::Color::Green); // Verde = Ayuda/Tamaño
    }
    
    // Origen al centro para facilitar colisiones
    mShape.setOrigin(15.f, 15.f);
}

sf::FloatRect PowerUp::getBounds() const {
    return mShape.getGlobalBounds();
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(mShape);
}

PowerUp::Type PowerUp::getType() const {
    return mType;
}
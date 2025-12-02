#include "PowerUp.hpp"

PowerUp::PowerUp(float x, float y, Type type) : mType(type), mLifetime(0.f) {
    mShape.setPosition(x, y);
    mShape.setRadius(15.f); // Un poco más grande que la pelota
    
    
    // Color según el tipo
    if (mType == SPEED_BOOST) {
        mShape.setFillColor(sf::Color::Blue);   // Rojo = Peligro/Velocidad
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

void PowerUp::update(sf::Time deltaTime) {
    mLifetime += deltaTime.asSeconds(); // Sumamos el tiempo que pasó

    // Efecto visual opcional: Parpadeo cuando va a desaparecer (si lleva más de 5s)
    if (mLifetime > 5.f) {
        // Un truco simple para parpadear: usar el módulo del tiempo
        // Si la parte decimal de mLifetime * 5 es > 0.5, lo mostramos, si no, ocultamos
        if (static_cast<int>(mLifetime * 10) % 2 == 0) {
            mShape.setFillColor(sf::Color::Transparent);
        } else {
            // Restaurar color original
            if (mType == SPEED_BOOST) mShape.setFillColor(sf::Color::Red);
            else mShape.setFillColor(sf::Color::Green);
        }
    }
}

bool PowerUp::isExpired() const {
    return mLifetime > 7.0f; // Desaparece a los 7 segundos
}
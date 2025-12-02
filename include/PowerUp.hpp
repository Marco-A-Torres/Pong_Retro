#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
public:
    // Tipos de poderes disponibles
    enum Type { SPEED_BOOST, PADDLE_ENLARGE };

    PowerUp(float x, float y, Type type);
    
    // Funciones básicas
    sf::FloatRect getBounds() const; // Para colisiones
    void draw(sf::RenderWindow& window);
    Type getType() const; // Para saber qué efecto aplicar

private:
    sf::CircleShape mShape;
    Type mType;
};
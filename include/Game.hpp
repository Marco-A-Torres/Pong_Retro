#pragma once
#include <SFML/Graphics.hpp>
#include "PowerUp.hpp"
#include <vector>

class Game {
public:
    Game();             // Constructor: Configura la ventana
    void run();         // Inicia el bucle principal

private:
    void processEvents(); // Detecta teclas y cierre de ventana
    void update(sf::Time deltaTime); // Actualiza posiciones (física)
    void render();        // Dibuja todo en pantalla

private:
    sf::RenderWindow mWindow; // La ventana del juego
    // Aquí agregaremos mPaddle, mBall, etc. en el futuro
    
    // --- ESTAS SON LAS LÍNEAS QUE TE FALTAN ---
    sf::RectangleShape mPaddleLeft;  
    sf::RectangleShape mPaddleRight;
    sf::CircleShape mBall;

    // --- NUEVAS VARIABLES PARA FÍSICA ----
    sf::Vector2f mBallVelocity; // Vector de velocidad (X, Y)
    float mPaddleSpeed;

    // --- SISTEMA DE POWER-UPS ---
    std::vector<PowerUp> mPowerUps; // Lista de powerups activos
    sf::Clock mPowerUpTimer;        // Reloj para respawn
    
    // Función auxiliar para generar uno nuevo
    void spawnPowerUp();
};
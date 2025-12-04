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
    
    // Palas y pelota
    sf::RectangleShape mPaddleLeft;  
    sf::RectangleShape mPaddleRight;
    sf::CircleShape mBall;


    // ----- ESTADOS Y UI ---
    enum State { MENU, PLAYING, GAME_OVER };
    State mGameState;

    sf::Font mFont;             // La tipografía
    sf::Text mScoreText;        // Marcador "0 - 0"
    sf::Text mMessageText;      // Mensajes "PRESS ENTER" o "PLAYER 1 WINS"
    
    int mScoreLeft;
    int mScoreRight;
    const int WINNING_SCORE = 5; // Gana el primero que llegue a 5

    // --- NUEVAS VARIABLES PARA FÍSICA ----
    sf::Vector2f mBallVelocity; // Vector de velocidad (X, Y)
    float mPaddleSpeed;

    // --- SISTEMA DE POWER-UPS ---
    std::vector<PowerUp> mPowerUps; // Lista de powerups activos
    sf::Clock mPowerUpTimer;        // Reloj para respawn
    
    // Función auxiliar para generar uno nuevo
    void spawnPowerUp();

    // --- LÓGICA DE PULIDO DE POWER-UPS ---
    sf::Clock mActiveEffectTimer;   // Cuenta cuánto tiempo lleva activo el poder
    bool mIsEffectActive;           // Bandera para saber si debemos checar el tiempo
    PowerUp::Type mCurrentEffect;   // Qué efecto está activo ahorita

    // --- VARIABLES PARA NUEVOS PODERES ---
    bool mControlsInvertedLeft;  // ¿Jugador Izq está maldito?
    bool mControlsInvertedRight; // ¿Jugador Der está maldito?
    
    sf::RectangleShape mWall;    // El objeto físico del muro
    
    // Función auxiliar para limpiar el desorden
    void resetEffects();
    // Para reiniciar score y posiciones
    void resetGame();
};
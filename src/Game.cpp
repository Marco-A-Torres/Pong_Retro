#include "Game.hpp"

// Configuración de la ventana (800x600 px)
Game::Game() : mWindow(sf::VideoMode(800, 600), "Pong Retro C++ - Avance 2") {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    mWindow.setFramerateLimit(60); // Limitamos a 60 FPS
    // --- ESTA ES LA PARTE QUE SEGURAMENTE FALTA ---
    
    // 1. Configurar Pala Izquierda
    mPaddleLeft.setSize(sf::Vector2f(20.f, 100.f)); // Tamaño: Ancho 20, Alto 100
    mPaddleLeft.setFillColor(sf::Color::White);     // Color
    mPaddleLeft.setPosition(50.f, 250.f);           // Posición (X, Y)

    // 2. Configurar Pala Derecha
    mPaddleRight.setSize(sf::Vector2f(20.f, 100.f));
    mPaddleRight.setFillColor(sf::Color::White);
    mPaddleRight.setPosition(730.f, 250.f);

    // 3. Configurar Pelota
    mBall.setRadius(10.f);                          // Radio 10
    mBall.setFillColor(sf::Color::White);
    mBall.setPosition(395.f, 295.f);                // Centro de pantalla aprox
    
    
    //4.- --- NUEVO: Configuración de Física ---
    mPaddleSpeed = 400.f;             // 400 pixeles por segundo
    mBallVelocity = {-200.f, -200.f}; // Se mueve a la izquierda y arriba (-X, -Y)
}

void Game::run() {
    sf::Clock clock; // Reloj para medir el tiempo entre frames
    
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart(); // Tiempo desde el último ciclo
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        
        // Aquí detectaremos inputs de teclado más adelante
    }
}

void Game::update(sf::Time deltaTime) {
    // 1. Movimiento de la Pala Izquierda (W / S)
    sf::Vector2f movementLeft(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movementLeft.y -= mPaddleSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movementLeft.y += mPaddleSpeed;
    
    mPaddleLeft.move(movementLeft * deltaTime.asSeconds());

    // 2. Movimiento de la Pala Derecha (Flechas)
    sf::Vector2f movementRight(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        movementRight.y -= mPaddleSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        movementRight.y += mPaddleSpeed;

    mPaddleRight.move(movementRight * deltaTime.asSeconds());

    // 3. Movimiento de la Pelota
    mBall.move(mBallVelocity * deltaTime.asSeconds());

    // 4. Colisiones con BORDES (Ventana)
    // Si toca el techo (Top < 0)
    if (mBall.getPosition().y < 0.f) {
        mBallVelocity.y = -mBallVelocity.y; // Invertir dirección Y (Rebotar)
        mBall.setPosition(mBall.getPosition().x, 0.f); // Corregir posición
    }
    // Si toca el suelo (Bottom > 600)
    // Restamos el diámetro (20) porque la posición es la esquina superior izq de la bola
    if (mBall.getPosition().y > 600.f - 20.f) {
        mBallVelocity.y = -mBallVelocity.y; 
        mBall.setPosition(mBall.getPosition().x, 580.f);
    }

    // 5. Colisión con PALA IZQUIERDA
    // Obtenemos los rectángulos invisibles que envuelven a los objetos
    if (mBall.getGlobalBounds().intersects(mPaddleLeft.getGlobalBounds())) {
        // Invertimos velocidad en X (Rebote horizontal)
        mBallVelocity.x = -mBallVelocity.x; 
        
        // Ajuste técnico: Empujar la pelota un poco para evitar que se quede "pegada" dentro de la pala
        mBall.setPosition(mPaddleLeft.getPosition().x + mPaddleLeft.getSize().x + 0.1f, mBall.getPosition().y);
    }

    // 6. Colisión con PALA DERECHA
    if (mBall.getGlobalBounds().intersects(mPaddleRight.getGlobalBounds())) {
        mBallVelocity.x = -mBallVelocity.x;
        
        // Ajuste técnico: Empujar hacia la izquierda
        mBall.setPosition(mPaddleRight.getPosition().x - mBall.getRadius() * 2 - 0.1f, mBall.getPosition().y);
    }

    // 7. Sistema de Puntuación (Reset)
    // Si sale por la Izquierda (Gol del Jugador Derecho)
    if (mBall.getPosition().x < -20.f) {
        mBall.setPosition(400.f, 300.f); // Volver al centro
        mBallVelocity = {-200.f, -200.f}; // Resetear velocidad
    }

    // Si sale por la Derecha (Gol del Jugador Izquierdo)
    if (mBall.getPosition().x > 820.f) {
        mBall.setPosition(400.f, 300.f);
        mBallVelocity = {200.f, -200.f}; // Saque para el otro lado
    }

    // --- LÓGICA DE POWER-UPS ---
    
    // 1. Generar nuevo PowerUp cada 5 segundos
    if (mPowerUpTimer.getElapsedTime().asSeconds() > 5.f) {
        spawnPowerUp();
        mPowerUpTimer.restart();
    }

    // 2. Checar colisiones: Pelota vs PowerUps
    // Usamos un loop inverso para poder borrar elementos sin romper el iterador
    for (int i = 0; i < mPowerUps.size(); ++i) {
        if (mBall.getGlobalBounds().intersects(mPowerUps[i].getBounds())) {
            
            // ¡COLISIÓN DETECTADA! Aplicar efecto
            PowerUp::Type type = mPowerUps[i].getType();
            
            if (type == PowerUp::SPEED_BOOST) {
                // Aumentar velocidad de la pelota un 50%
                mBallVelocity.x *= 1.5f;
                mBallVelocity.y *= 1.5f;
            } 
            else if (type == PowerUp::PADDLE_ENLARGE) {
                // Agrandar ambas palas (para mantenerlo justo/caótico)
                mPaddleLeft.setSize(sf::Vector2f(20.f, 150.f));
                mPaddleRight.setSize(sf::Vector2f(20.f, 150.f));
            }
            
            // Eliminar el powerup recolectado del vector
            mPowerUps.erase(mPowerUps.begin() + i);
            i--; // Ajustar índice tras borrar
        }
    }
}

void Game::spawnPowerUp() {
    // Posición aleatoria dentro del campo (evitando bordes)
    float x = 100 + (std::rand() % 600); // Entre 100 y 700
    float y = 50 + (std::rand() % 500);  // Entre 50 y 550
    
    // Tipo aleatorio (0 o 1)
    int typeInt = std::rand() % 2;
    PowerUp::Type type = (typeInt == 0) ? PowerUp::SPEED_BOOST : PowerUp::PADDLE_ENLARGE;
    
    // Crear y guardar en el vector (push_back)
    mPowerUps.emplace_back(x, y, type);
}

void Game::render() {
    mWindow.clear(sf::Color::Black); 
    
    // --- VERIFICA QUE TENGAS ESTAS 3 LÍNEAS ---
    mWindow.draw(mPaddleLeft);
    mWindow.draw(mPaddleRight);
    mWindow.draw(mBall);
    
    // --- DIBUJAR POWERUPS ---
    // Recorremos el vector y dibujamos cada uno
    for (auto& powerUp : mPowerUps) {
        powerUp.draw(mWindow);
    }
    
    mWindow.display();
}
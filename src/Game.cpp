#include "Game.hpp"
#include <iostream> // Agrega esto arriba de todo en Game.cpp

// Configuración de la ventana (800x600 px)
Game::Game() : mWindow(sf::VideoMode(800, 600), "Pong Retro C++ - Version Final") {
    mGameState = MENU;
    mScoreLeft = 0;   // Limpiar basura
    mScoreRight = 0;  // Limpiar basura
    
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    mWindow.setFramerateLimit(60); // Limitamos a 60 FPS
    
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
    
    // --- CARGAR FUENTE Y TEXTO ---
    // IMPORTANTE: Asegúrate de tener el archivo assets/fonts/font.ttf
    // En el constructor Game::Game...
    
    // Intenta cargar e imprime en la consola si falla
    if (!mFont.loadFromFile("assets/fonts/font.ttf")) {
        std::cerr << "ERROR CRITICO: No se encontro el archivo assets/fonts/font.ttf" << std::endl;
        std::cerr << "Asegurate de que la carpeta assets este JUNTO al ejecutable." << std::endl;
    } else {
        std::cout << "EXITO: Fuente cargada correctamente." << std::endl;
    }

    // Configurar Marcador
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(40);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition(300.f, 20.f);
    mScoreText.setString("0 - 0");

    // Configurar Mensaje Central
    mMessageText.setFont(mFont);
    mMessageText.setCharacterSize(30);
    mMessageText.setFillColor(sf::Color::Yellow);
    mMessageText.setPosition(125.f, 250.f);
    mMessageText.setString("PRESS ENTER TO START");
    
    //4.- --- NUEVO: Configuración de Física ---
    mPaddleSpeed = 400.f;             // 400 pixeles por segundo
    mBallVelocity = {-200.f, -200.f}; // Se mueve a la izquierda y arriba (-X, -Y)

    //Power up maldición
    mControlsInvertedLeft = false;
    mControlsInvertedRight = false;

    // Configurar el Muro (inicialmente oculto fuera de pantalla)
    mWall.setSize(sf::Vector2f(20.f, 300.f)); // Un muro alto
    mWall.setFillColor(sf::Color(0, 0, 255, 150)); // Azul semitransparente
    mWall.setOrigin(10.f, 150.f); // Centro del muro
    mWall.setPosition(-100.f, -100.f); // Fuera de juego
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
        // Cerrar ventana
        if (event.type == sf::Event::Closed)
            mWindow.close();

        // Detectar pulsación de teclas (Solo una vez al bajar el dedo)
        if (event.type == sf::Event::KeyPressed) {
            
            // Si presionan ENTER
            if (event.key.code == sf::Keyboard::Enter) {
                
                // Caso 1: Estamos en Menú -> Empezar a jugar
                if (mGameState == MENU) {
                    mGameState = PLAYING;
                }
                // Caso 2: Estamos en Game Over -> Reiniciar todo e ir al Menú
                else if (mGameState == GAME_OVER) {
                    resetGame(); 
                    mGameState = MENU;
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {

    // --- NUEVO: SI NO ESTAMOS JUGANDO, NO HACEMOS NADA ---
    if (mGameState != PLAYING) {
        return; // Salimos de la función inmediatamente
    }

    // Velocidad base
    float speedLeft = mPaddleSpeed;
    float speedRight = mPaddleSpeed;

    // Si están malditos, invertimos su velocidad individual
    if (mControlsInvertedLeft) speedLeft = -mPaddleSpeed;
    if (mControlsInvertedRight) speedRight = -mPaddleSpeed;

    // Movimiento Pala Izquierda (Usamos speedLeft)
    sf::Vector2f movementLeft(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movementLeft.y -= speedLeft;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movementLeft.y += speedLeft;
    mPaddleLeft.move(movementLeft * deltaTime.asSeconds());

    // Movimiento Pala Derecha (Usamos speedRight)
    sf::Vector2f movementRight(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movementRight.y -= speedRight;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movementRight.y += speedRight;
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

    // Colisión Pelota vs Muro (Solo si el muro está activo/en pantalla)
    if (mIsEffectActive && mCurrentEffect == PowerUp::WALL) {
        if (mBall.getGlobalBounds().intersects(mWall.getGlobalBounds())) {
            mBallVelocity.x = -mBallVelocity.x; // ¡Rebota!
            
            // Sonido visual: Cambiar color del muro brevemente
            mWall.setFillColor(sf::Color::White); 
        } else {
            mWall.setFillColor(sf::Color(0, 0, 255, 150)); // Volver a azul
        }
    }

    // 7. Sistema de Puntuación (Reset)
    // Si sale por la Izquierda (Gol del Derecho)
    if (mBall.getPosition().x < -20.f) {
        mScoreRight++;
        mScoreText.setString(std::to_string(mScoreLeft) + " - " + std::to_string(mScoreRight));
        resetEffects(); // Tu función de limpiar powerups
        
        // Checar victoria
        if (mScoreRight >= WINNING_SCORE) {
            mGameState = GAME_OVER;
            mMessageText.setString("PLAYER 2 WINS!\nPress Enter");
            mMessageText.setPosition(200.f, 200.f);
        } else {
            // Reset normal
            mBall.setPosition(400.f, 300.f);
            mBallVelocity = {-200.f, -200.f};
        }
    }

    // Si sale por la Derecha (Gol del Izquierdo)
    if (mBall.getPosition().x > 820.f) {
        mScoreLeft++;
        mScoreText.setString(std::to_string(mScoreLeft) + " - " + std::to_string(mScoreRight));
        resetEffects();

        if (mScoreLeft >= WINNING_SCORE) {
            mGameState = GAME_OVER;
            mMessageText.setString("PLAYER 1 WINS!\nPress Enter");
            mMessageText.setPosition(200.f, 200.f);
        } else {
            mBall.setPosition(400.f, 300.f);
            mBallVelocity = {200.f, -200.f};
        }
    }

    // --- LÓGICA DE POWER-UPS ---
    
    // 1. Generar nuevo PowerUp cada 5 segundos
    if (mPowerUpTimer.getElapsedTime().asSeconds() > 5.f) {
        spawnPowerUp();
        mPowerUpTimer.restart();
    }

    // 2. Checar colisiones: Pelota vs PowerUps
    // Usamos un loop inverso para poder borrar elementos sin romper el iterador
    // DENTRO DE UPDATE (Loop de colisión con PowerUps)
    for (int i = 0; i < mPowerUps.size(); ++i) {
        // 1. ACTUALIZAR TIEMPO DE VIDA
        mPowerUps[i].update(deltaTime);

        // 2. VERIFICAR SI EXPIRÓ (Murió de viejo)
        if (mPowerUps[i].isExpired()) {
            mPowerUps.erase(mPowerUps.begin() + i);
            i--; // Ajustamos el índice porque borramos un elemento
            continue; // Saltamos a la siguiente iteración
        }
        if (mBall.getGlobalBounds().intersects(mPowerUps[i].getBounds())) {
            
            // 1. PRIMERO: Limpiar cualquier efecto anterior para no bugear
            resetEffects(); 

            PowerUp::Type type = mPowerUps[i].getType();
            mIsEffectActive = true;
            mActiveEffectTimer.restart(); // Inicia el cronómetro del poder
            mCurrentEffect = type;
            
            // Dentro del loop de colisión con powerups en update()

            if (type == PowerUp::SPEED_BOOST) {
                mBall.setFillColor(sf::Color::Red);
                mBallVelocity.x *= 1.5f;
                mBallVelocity.y *= 1.5f;
            } 
            else if (type == PowerUp::PADDLE_ENLARGE) {
                // Agrandar al que tenga la pelota cerca
                if (mBall.getPosition().x < 400) mPaddleLeft.setSize(sf::Vector2f(20.f, 200.f));
                else mPaddleRight.setSize(sf::Vector2f(20.f, 200.f));
            }
            else if (type == PowerUp::GHOST_BALL) {
                // 1. FANTASMA: Volver transparente
                mBall.setFillColor(sf::Color(255, 255, 255, 30)); // Casi invisible
            }
            else if (type == PowerUp::REVERSE_CONTROLS) {
                // 2. MALDICIÓN: Invertir al oponente
                // Si la pelota va a la derecha (>0), ataca el Izq -> Maldice al Der
                if (mBallVelocity.x > 0) mControlsInvertedRight = true;
                else mControlsInvertedLeft = true;
            }
            else if (type == PowerUp::WALL) {
                // 3. MURO: Ponerlo en el centro
                mWall.setPosition(400.f, 300.f);
            }
            
            mPowerUps.erase(mPowerUps.begin() + i);
            i--; 
        }
    }

    // --- GESTIÓN DE DURACIÓN DE EFECTOS ---
    if (mIsEffectActive) {
        // Si pasaron 5 segundos
        if (mActiveEffectTimer.getElapsedTime().asSeconds() > 5.f) {
            resetEffects();
            
            // Truco de física: Si la pelota estaba muy rápida, hay que frenarla
            // Simplemente reducimos el vector actual un poco
            if (mCurrentEffect == PowerUp::SPEED_BOOST) {
                mBallVelocity.x *= 0.6f; // Frenado suave
                mBallVelocity.y *= 0.6f;
            }
        }
    }

    // Si sale por la Izquierda
    if (mBall.getPosition().x < -20.f) {
        resetEffects(); // <--- AGREGAR ESTO
        mBall.setPosition(400.f, 300.f);
        mBallVelocity = {-200.f, -200.f}; 
    }
}

void Game::spawnPowerUp() {
    // Posición aleatoria dentro del campo (evitando bordes)
    float x = 100 + (std::rand() % 600); // Entre 100 y 700
    float y = 50 + (std::rand() % 500);  // Entre 50 y 550
    
    // Tipo aleatorio (0 o 1)
    int typeInt = std::rand() % 5; 
    PowerUp::Type type = static_cast<PowerUp::Type>(typeInt);
    
    mPowerUps.emplace_back(x, y, type);
}

void Game::resetEffects() {
    mIsEffectActive = false;
    mBall.setFillColor(sf::Color::White); // Quita color rojo y fantasma
    
    mPaddleLeft.setSize(sf::Vector2f(20.f, 100.f));
    mPaddleRight.setSize(sf::Vector2f(20.f, 100.f));
    
    // Apagar maldiciones
    mControlsInvertedLeft = false;
    mControlsInvertedRight = false;

    // Quitar muro
    mWall.setPosition(-100.f, -100.f);
}

void Game::render() {
    mWindow.clear(sf::Color::Black);

    if (mGameState == MENU) {
        mMessageText.setString("PONG RETRO C++\n\nPress Enter to Start");
        
        // --- TRUCO DE CENTRADO ---
        // 1. Obtener la caja que envuelve al texto
        sf::FloatRect textRect = mMessageText.getLocalBounds();
        // 2. Poner el punto de anclaje (Origin) en el centro matemático del texto
        mMessageText.setOrigin(textRect.left + textRect.width/2.0f,
                               textRect.top  + textRect.height/2.0f);
        // 3. Mover ese punto de anclaje al centro de la pantalla (400, 300)
        mMessageText.setPosition(400.f, 300.f);
        
        mWindow.draw(mMessageText);
    }
    else if (mGameState == PLAYING) {
        // Dibujar Juego
        mWindow.draw(mScoreText); // El marcador arriba
        
        // La línea divisoria (Opcional, estética)
        sf::RectangleShape line(sf::Vector2f(2.f, 600.f));
        line.setPosition(400.f, 0.f);
        line.setFillColor(sf::Color(100, 100, 100));
        mWindow.draw(line);

        mWindow.draw(mPaddleLeft);
        mWindow.draw(mPaddleRight);
        
        // Dibujar PowerUps (Tu código)
        for (auto& p : mPowerUps) p.draw(mWindow);
        
        // Dibujar Muro (Si existe)
        if (mIsEffectActive && mCurrentEffect == PowerUp::WALL) mWindow.draw(mWall);

        mWindow.draw(mBall);
    }
    else if (mGameState == GAME_OVER) {
        // El texto ya se seteó en update, pero aseguramos el centrado aquí también
        sf::FloatRect textRect = mMessageText.getLocalBounds();
        mMessageText.setOrigin(textRect.left + textRect.width/2.0f,
                               textRect.top  + textRect.height/2.0f);
        mMessageText.setPosition(400.f, 300.f);
        
        mWindow.draw(mMessageText); 
    }

    mWindow.display();
}

void Game::resetGame() {
    // 1. Reiniciar Marcador
    mScoreLeft = 0;
    mScoreRight = 0;
    mScoreText.setString("0 - 0");

    // 2. Reiniciar Posiciones
    mBall.setPosition(400.f, 300.f);
    mBallVelocity = {-200.f, -200.f}; // Saque inicial
    
    mPaddleLeft.setPosition(50.f, 250.f);
    mPaddleRight.setPosition(730.f, 250.f);

    // 3. Limpiar PowerUps y Efectos
    resetEffects();     // Quita velocidad roja, muros, etc.
    mPowerUps.clear();  // Borra las bolitas que quedaron flotando
}
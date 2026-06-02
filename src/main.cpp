#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>
#include <vector>

enum EstadoJuego { MENU, JUGANDO, COMO_JUGAR, CONFIGURACION, FIN_PARTIDA };
enum EstadoPersonaje { IDLE, ATACANDO, ESPECIAL };

struct Nota {
    sf::RectangleShape rect;
    float velocidad;
    int riel;       
    int propietario; 
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "KOF x OsuMania - Duelo por Turnos");
    window.setFramerateLimit(60);

    // Cargar la hoja de sprites
    sf::Texture texturaNaruto;
    bool hojaCargada = true;
    if (!texturaNaruto.loadFromFile("assets/imagenes/naruto.png")) {
        if (!texturaNaruto.loadFromFile("imagenes/naruto.png")) {
            hojaCargada = false;
        }
    }

    // --- RECTÁNGULOS DE ANIMACIÓN ---
    sf::IntRect cuadroNormal(35, 275, 130, 160); 
    sf::IntRect cuadrosEspecial[2] = { sf::IntRect(220, 275, 140, 160), sf::IntRect(410, 275, 150, 160) };
    sf::IntRect cuadrosAtaque[5] = {             
        sf::IntRect(35,  25, 145, 160), sf::IntRect(205, 25, 150, 160),
        sf::IntRect(400, 25, 150, 160), sf::IntRect(560, 20, 250, 165), sf::IntRect(810, 25, 150, 160)
    };

    // Configurar Sprites
    sf::Sprite p1Sprite;
    if (hojaCargada) {
        p1Sprite.setTexture(texturaNaruto);
        p1Sprite.setTextureRect(cuadroNormal); 
        p1Sprite.setPosition(30.f, 280.f); 
        p1Sprite.setScale(1.6f, 1.6f);
    }

    sf::Sprite p2Sprite;
    if (hojaCargada) {
        p2Sprite.setTexture(texturaNaruto);
        p2Sprite.setTextureRect(cuadroNormal);
        p2Sprite.setPosition(770.f, 280.f); 
        p2Sprite.setScale(-1.6f, 1.6f); 
    }

    // Estados de personajes y puntuaciones
    EstadoPersonaje p1Estado = IDLE; int p1Cuadro = 0; sf::Clock p1Reloj; int comboP1 = 0; int scoreP1 = 0;
    EstadoPersonaje p2Estado = IDLE; int p2Cuadro = 0; sf::Clock p2Reloj; int comboP2 = 0; int scoreP2 = 0;

    // Sistema Osu!Mania
    std::vector<Nota> notas;
    sf::Clock relojNotas;

    // --- VARIABLES DEL SISTEMA DE TURNOS ---
    int jugadorActivo = 1;          // 1 = Turno P1, 2 = Turno P2
    int turnosRestantesPorJugador = 5; 
    float tiempoLimiteTurno = 60.f;  // 1 minuto (60 segundos) por turno
    sf::Clock relojTurno;

    // Pistas Centradas
    sf::RectangleShape pistasOsu[4];
    float xCentroInicio = 300.f; 
    for (int i = 0; i < 4; i++) {
        pistasOsu[i].setSize(sf::Vector2f(45.f, 480.f));
        pistasOsu[i].setFillColor(sf::Color(40, 40, 48));
        pistasOsu[i].setOutlineColor(sf::Color(55, 55, 65));
        pistasOsu[i].setOutlineThickness(1.f);
        pistasOsu[i].setPosition(xCentroInicio + (i * 50), 20.f);
    }

    sf::RectangleShape lineaJuicio(sf::Vector2f(200.f, 6.f));
    lineaJuicio.setFillColor(sf::Color::Green);
    lineaJuicio.setPosition(xCentroInicio, 500.f);

    sf::RectangleShape suelo(sf::Vector2f(800.f, 120.f));
    suelo.setFillColor(sf::Color(30, 30, 35));
    suelo.setPosition(0.f, 480.f);

    // Fuentes y Textos
    sf::Font font;
    if (!font.loadFromFile("assets/fuentes/font.ttf")) {
        if (!font.loadFromFile("fuentes/font.ttf")) {
            font.loadFromFile("C:/Windows/Fonts/arial.ttf"); 
        }
    }

    // Menú Principal
    sf::Text titulo("Leyenda del Ritmo", font, 65);
    titulo.setFillColor(sf::Color::Yellow);
    titulo.setPosition(140.f, 60.f);
    titulo.setOutlineColor(sf::Color::Black);
    titulo.setOutlineThickness(4.f);

    std::string opciones[] = {"Modo Turnos (5 Rondas)", "Como jugar", "Configuracion"};
    sf::Text textosMenu[3];
    int opcionSeleccionada = 0;
    for (int i = 0; i < 3; i++) {
        textosMenu[i].setFont(font);
        textosMenu[i].setString(opciones[i]);
        textosMenu[i].setCharacterSize(40);
        textosMenu[i].setPosition(240.f, 250.f + (i * 80));
        textosMenu[i].setOutlineColor(sf::Color::Black);
        textosMenu[i].setOutlineThickness(3.f);
    }

    // UI del Juego
    sf::Text textoComboP1("", font, 22); textoComboP1.setPosition(30.f, 30.f);
    sf::Text textoComboP2("", font, 22); textoComboP2.setPosition(560.f, 30.f);
    
    sf::Text textoCronometro("", font, 35);
    textoCronometro.setPosition(365.f, 540.f);
    textoCronometro.setFillColor(sf::Color::Red);

    sf::Text textoTurnoAlerta("", font, 30);
    textoTurnoAlerta.setOutlineColor(sf::Color::Black);
    textoTurnoAlerta.setOutlineThickness(2.f);

    sf::Text textoVolver("Presione ESC para volver al menu", font, 20);
    textoVolver.setFillColor(sf::Color::White);
    textoVolver.setPosition(20.f, 550.f);

    EstadoJuego estadoActual = MENU;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (estadoActual == MENU)
                {
                    if (event.key.code == sf::Keyboard::Up) opcionSeleccionada = (opcionSeleccionada - 1 + 3) % 3;
                    else if (event.key.code == sf::Keyboard::Down) opcionSeleccionada = (opcionSeleccionada + 1) % 3;
                    else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
                    {
                        if (opcionSeleccionada == 0) {
                            estadoActual = JUGANDO;
                            comboP1 = 0; scoreP1 = 0;
                            comboP2 = 0; scoreP2 = 0;
                            jugadorActivo = 1;
                            turnosRestantesPorJugador = 5;
                            notas.clear();
                            relojTurno.restart();
                            relojNotas.restart();
                        }
                        else if (opcionSeleccionada == 1) estadoActual = COMO_JUGAR;
                        else if (opcionSeleccionada == 2) estadoActual = CONFIGURACION;
                    }
                }
                else 
                {
                    if (event.key.code == sf::Keyboard::Escape) estadoActual = MENU;

                    if (estadoActual == JUGANDO) {
                        // CAPTURA DE TECLAS FILTRADA POR EL JUGADOR ACTIVO
                        if (jugadorActivo == 1) {
                            int riel = -1;
                            if (event.key.code == sf::Keyboard::A) riel = 0;
                            if (event.key.code == sf::Keyboard::S) riel = 1;
                            if (event.key.code == sf::Keyboard::K) riel = 2;
                            if (event.key.code == sf::Keyboard::L) riel = 3;

                            if (riel != -1) {
                                if (p1Estado != ATACANDO) { p1Estado = ATACANDO; p1Cuadro = 0; p1Reloj.restart(); }
                                for (size_t i = 0; i < notas.size(); i++) {
                                    if (notas[i].riel == riel && notas[i].propietario == 1) {
                                        if (std::abs(notas[i].rect.getPosition().y - 500.f) < 45.f) {
                                            comboP1++; scoreP1 += 10;
                                            notas.erase(notas.begin() + i); break;
                                        }
                                    }
                                }
                            }
                        }
                        else if (jugadorActivo == 2) {
                            int riel = -1;
                            if (event.key.code == sf::Keyboard::Left)  riel = 0;
                            if (event.key.code == sf::Keyboard::Down)  riel = 1;
                            if (event.key.code == sf::Keyboard::Up)    riel = 2;
                            if (event.key.code == sf::Keyboard::Right) riel = 3;

                            if (riel != -1) {
                                if (p2Estado != ATACANDO) { p2Estado = ATACANDO; p2Cuadro = 0; p2Reloj.restart(); }
                                for (size_t i = 0; i < notas.size(); i++) {
                                    if (notas[i].riel == riel && notas[i].propietario == 2) {
                                        if (std::abs(notas[i].rect.getPosition().y - 500.f) < 45.f) {
                                            comboP2++; scoreP2 += 10;
                                            notas.erase(notas.begin() + i); break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // --- LÓGICA DE ACTUALIZACIÓN ---
        if (estadoActual == JUGANDO)
        {
            float tiempoTranscurrido = relojTurno.getElapsedTime().asSeconds();
            float tiempoRestante = tiempoLimiteTurno - tiempoTranscurrido;

            if (tiempoRestante <= 0.f) {
                // CONTROLADOR DEL CAMBIO DE TURNO
                notas.clear(); // Limpiar rastro de notas previas
                if (jugadorActivo == 1) {
                    jugadorActivo = 2; 
                } else {
                    jugadorActivo = 1;
                    turnosRestantesPorJugador--; // Una ronda completa menos
                }
                
                if (turnosRestantesPorJugador <= 0) {
                    estadoActual = FIN_PARTIDA;
                }
                relojTurno.restart();
                tiempoRestante = tiempoLimiteTurno;
            }

            // Generador de Notas exclusivo para el Jugador del turno actual
            if (relojNotas.getElapsedTime().asSeconds() > 0.4f)
            {
                Nota nuevaNota;
                nuevaNota.rect.setSize(sf::Vector2f(45.f, 20.f));
                nuevaNota.riel = rand() % 4;
                nuevaNota.propietario = jugadorActivo;

                if (jugadorActivo == 1) nuevaNota.rect.setFillColor(sf::Color::Cyan);
                else nuevaNota.rect.setFillColor(sf::Color(255, 0, 255));

                nuevaNota.rect.setPosition(xCentroInicio + (nuevaNota.riel * 50), 0.f);
                nuevaNota.velocidad = 6.0f;
                notas.push_back(nuevaNota);
                relojNotas.restart();
            }

            for (size_t i = 0; i < notas.size(); i++) {
                notas[i].rect.move(0.f, notas[i].velocidad);
            }

            // Penalización por dejar pasar notas
            if (!notas.empty() && notas[0].rect.getPosition().y > 550.f) {
                if (notas[0].propietario == 1) comboP1 = 0;
                else comboP2 = 0;
                notas.erase(notas.begin());
            }

            // Cambios automáticos a modo Especial por racha
            if (comboP1 >= 30 && p1Estado == IDLE) p1Estado = ESPECIAL;
            if (comboP2 >= 30 && p2Estado == IDLE) p2Estado = ESPECIAL;

            // Animación Sprites
            if (hojaCargada) {
                if (p1Estado == IDLE) p1Sprite.setTextureRect(cuadroNormal);
                else if (p1Estado == ATACANDO) {
                    if (p1Reloj.getElapsedTime().asSeconds() > 0.06f) {
                        p1Cuadro++; if (p1Cuadro >= 5) { p1Estado = (comboP1 >= 30) ? ESPECIAL : IDLE; p1Cuadro = 0; }
                        p1Reloj.restart();
                    }
                    p1Sprite.setTextureRect(cuadrosAtaque[p1Cuadro]);
                }
                else if (p1Estado == ESPECIAL) {
                    if (p1Reloj.getElapsedTime().asSeconds() > 0.12f) { p1Cuadro = (p1Cuadro + 1) % 2; p1Reloj.restart(); }
                    p1Sprite.setTextureRect(cuadrosEspecial[p1Cuadro]);
                }

                if (p2Estado == IDLE) p2Sprite.setTextureRect(cuadroNormal);
                else if (p2Estado == ATACANDO) {
                    if (p2Reloj.getElapsedTime().asSeconds() > 0.06f) {
                        p2Cuadro++; if (p2Cuadro >= 5) { p2Estado = (comboP2 >= 30) ? ESPECIAL : IDLE; p2Cuadro = 0; }
                        p2Reloj.restart();
                    }
                    p2Sprite.setTextureRect(cuadrosAtaque[p2Cuadro]);
                }
                else if (p2Estado == ESPECIAL) {
                    if (p2Reloj.getElapsedTime().asSeconds() > 0.12f) { p2Cuadro = (p2Cuadro + 1) % 2; p2Reloj.restart(); }
                    p2Sprite.setTextureRect(cuadrosEspecial[p2Cuadro]);
                }
            }

            // Actualizar textos UI
            textoCronometro.setString(std::to_string(static_cast<int>(tiempoRestante)) + "s");
            
            if (jugadorActivo == 1) {
                textoTurnoAlerta.setString("TURNO: JUGADOR 1");
                textoTurnoAlerta.setFillColor(sf::Color::Cyan);
                textoTurnoAlerta.setPosition(290.f, 230.f);
            } else {
                textoTurnoAlerta.setString("TURNO: JUGADOR 2");
                textoTurnoAlerta.setFillColor(sf::Color(255, 0, 255));
                textoTurnoAlerta.setPosition(290.f, 230.f);
            }
        }

        if (estadoActual == MENU) {
            for (int i = 0; i < 3; i++) textosMenu[i].setFillColor(i == opcionSeleccionada ? sf::Color::Red : sf::Color::White);
        }

        // --- RENDERIZADO ---
        window.clear(sf::Color(20, 20, 28));

        switch (estadoActual)
        {
            case MENU:
                window.draw(titulo);
                for (int i = 0; i < 3; i++) window.draw(textosMenu[i]);
                break;

            case JUGANDO:
                window.draw(suelo);
                for (int i = 0; i < 4; i++) window.draw(pistasOsu[i]);
                window.draw(lineaJuicio);
                for (const auto& nota : notas) window.draw(nota.rect);
                
                if (hojaCargada) {
                    window.draw(p1Sprite);
                    window.draw(p2Sprite);
                }

                textoComboP1.setString("P1 SCORE: " + std::to_string(scoreP1) + "\nCOMBO: " + std::to_string(comboP1));
                textoComboP1.setFillColor(sf::Color::Cyan);
                
                textoComboP2.setString("P2 SCORE: " + std::to_string(scoreP2) + "\nCOMBO: " + std::to_string(comboP2));
                textoComboP2.setFillColor(sf::Color(255, 0, 255));

                window.draw(textoComboP1);
                window.draw(textoComboP2);
                window.draw(textoCronometro);
                
                // Mostrar alerta visual de quién posee el turno si el tiempo es alto
                if (relojTurno.getElapsedTime().asSeconds() < 3.0f) {
                    window.draw(textoTurnoAlerta);
                }

                {
                    sf::Text txtRondas("Rondas restantes: " + std::to_string(turnosRestantesPorJugador), font, 18);
                    txtRondas.setPosition(315.f, 50.f); txtRondas.setFillColor(sf::Color::White);
                    window.draw(txtRondas);
                }
                break;

            case FIN_PARTIDA:
                {
                    sf::Text txtFin("¡PARTIDA TERMINADA!", font, 45);
                    txtFin.setFillColor(sf::Color::Yellow); txtFin.setPosition(160.f, 150.f);
                    
                    std::string ganador = (scoreP1 > scoreP2) ? "JUGADOR 1 (CYAN)" : (scoreP2 > scoreP1) ? "JUGADOR 2 (MAGENTA)" : "EMPATE PERFECTO";
                    sf::Text txtGanador("Ganador: " + ganador + "\n\nScore P1: " + std::to_string(scoreP1) + "\nScore P2: " + std::to_string(scoreP2), font, 28);
                    txtGanador.setFillColor(sf::Color::White); txtGanador.setPosition(180.f, 250.f);

                    window.draw(txtFin); window.draw(txtGanador); window.draw(textoVolver);
                }
                break;

            case COMO_JUGAR:
                {
                    sf::Text txtInfo("DUELO POR TURNOS (ESTILO KOF):\n\n- Cada jugador tiene 1 MINUTO exclusivo en la pista.\n- Las notas de tu rival no apareceran hasta su turno.\n\n- JUGADOR 1 (Teclas: A, S, K, L) - Notas Cyan.\n- JUGADOR 2 (Teclas: Flechas) - Notas Magenta.\n\n- Se juegan un total de 5 Turnos/Rondas por jugador.", font, 22);
                    txtInfo.setFillColor(sf::Color::White); txtInfo.setPosition(60.f, 150.f);
                    window.draw(txtInfo); window.draw(textoVolver);
                }
                break;

            case CONFIGURACION:
                {
                    sf::Text txtConf("CONFIGURACION:\n\nTiempo por Turno: 60 Segundos\nTotal de Rondas: 5 Rondas por Jugador\n\n[Controles J1]: A, S, K, L\n[Controles J2]: Flecha Izq, Abajo, Arriba, Der", font, 22);
                    txtConf.setFillColor(sf::Color::White); txtConf.setPosition(60.f, 180.f);
                    window.draw(txtConf); window.draw(textoVolver);
                }
                break;
        }

        window.display();
    }

    return 0;
}

#include <raylib.h>
#include <string>
#include "vocalista.h"
#include "guitarrista.h"
#include "baterista.h"
#include "banda.h"

int main() {
    const int anchoPantalla = 1280;
    const int altoPantalla = 720;
    InitWindow(anchoPantalla, altoPantalla, "🎤 KOF ROCK - Torneo de Bandas Grafico");
    SetTargetFPS(60);

    // 🌟 Corregido: Pasamos "" en la ruta del archivo para que no busque nada en disco
    Personaje* luffo = new Vocalista("Luffo", "Una Pieza", 100, 16, true, "", 85);
    Personaje* nauto = new Vocalista("Nauto", "Naranja Ninja", 100, 15, true, "", 80); 
    Personaje* ichigox = new Guitarrista("Ichigox", "Lejia", 100, 17, false, "", 88);

    Personaje* hitorix = new Guitarrista("Hitorix", "Bochi la Piedra!", 100, 20, true, "", 95);
    Personaje* nijikax = new Baterista("Nijikax", "Bochi la Piedra!", 100, 14, true, "", 80);
    Personaje* ryox = new Vocalista("Ryox", "Bochi la Piedra!", 100, 13, false, "", 70);

    Banda* bandaPlayer = new Banda("Los Shonen Kings");
    bandaPlayer->agregarIntegrante(luffo);
    bandaPlayer->agregarIntegrante(nauto);
    bandaPlayer->agregarIntegrante(ichigox);
    bandaPlayer->calcularSinergia();

    Banda* bandaRival = new Banda("Las Chicas de la Piedra");
    bandaRival->agregarIntegrante(hitorix);
    bandaRival->agregarIntegrante(nijikax);
    bandaRival->agregarIntegrante(ryox);
    bandaRival->calcularSinergia();

    int moralP1 = 100;
    int moralP2 = 100;
    float tiempoEfecto = 0.0f;
    bool conciertoActivo = true;
    std::string mensajeAlerta = "¡Presiona [J], [K] o [L] para tocar!";

    while (!WindowShouldClose()) {
        if (tiempoEfecto > 0.0f) {
            tiempoEfecto -= GetFrameTime();
            if (tiempoEfecto <= 0.0f) {
                for (Personaje* p : bandaPlayer->getIntegrantes()) p->setAtacando(false);
            }
        }

        if (conciertoActivo) {
            int danioBase = 10;
            int danioFinal = danioBase + (bandaPlayer->getSinergiaTotal() / 5);

            if (IsKeyPressed(KEY_J)) {
                bandaPlayer->getIntegrantes()[0]->setAtacando(true);
                moralP2 -= danioFinal;
                tiempoEfecto = 0.5f;
                mensajeAlerta = "💥 ¡Ataque del Vocalista! Danio: " + std::to_string(danioFinal);
            }
            else if (IsKeyPressed(KEY_K)) {
                bandaPlayer->getIntegrantes()[1]->setAtacando(true);
                moralP2 -= (danioFinal - 2);
                tiempoEfecto = 0.5f;
                mensajeAlerta = "🎸 ¡Solo de Guitarra! Danio: " + std::to_string(danioFinal - 2);
            }
            else if (IsKeyPressed(KEY_L)) {
                bandaPlayer->getIntegrantes()[2]->setAtacando(true);
                moralP1 += 15;
                if (moralP1 > 100) moralP1 = 100;
                tiempoEfecto = 0.5f;
                mensajeAlerta = "🛡️ ¡Ritmo solido! Tu banda recupero +15 de moral.";
            }

            if (moralP2 <= 0) {
                moralP2 = 0;
                conciertoActivo = false;
                mensajeAlerta = "🏆 ¡VICTORIA TOTAL! Dominaste el escenario.";
            }
            
            if (IsKeyPressed(KEY_J) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_L)) {
                if (conciertoActivo) {
                    int danioRival = 8 + (bandaRival->getVelocidadBanda() / 40);
                    moralP1 -= danioRival;
                    if (moralP1 <= 0) {
                        moralP1 = 0;
                        conciertoActivo = false;
                        mensajeAlerta = "💀 GAME OVER. Tu banda fue abucheada.";
                    }
                }
            }
        }

        BeginDrawing();
            ClearBackground(DARKGRAY);

            bandaPlayer->dibujarBanda(100.0f, 350.0f);  
            bandaRival->dibujarBanda(750.0f, 350.0f);   

            DrawText(bandaPlayer->getNombreBanda().c_str(), 50, 40, 20, LIGHTGRAY);
            DrawRectangle(50, 70, 400, 30, RED);
            DrawRectangle(50, 70, moralP1 * 4, 30, LIME);
            DrawText(TextFormat("%d / 100", moralP1), 60, 75, 18, BLACK);

            DrawText(bandaRival->getNombreBanda().c_str(), 830, 40, 20, LIGHTGRAY);
            DrawRectangle(830, 70, 400, 30, RED);
            DrawRectangle(830, 70, moralP2 * 4, 30, ORANGE);
            DrawText(TextFormat("%d / 100", moralP2), 840, 75, 18, BLACK);

            DrawRectangle(50, 620, 1180, 60, Fade(BLACK, 0.6f));
            DrawText(mensajeAlerta.c_str(), 80, 640, 22, WHITE);
            
            if (conciertoActivo) {
                DrawText("Controles: [J] Vocalista | [K] Guitarra | [L] Bateria", 400, 20, 18, GOLD);
            } else {
                DrawText("Presiona [ESC] para salir del evento", 480, 20, 18, RED);
            }
        EndDrawing();
    }

    delete bandaPlayer;
    delete bandaRival;
    CloseWindow();
    return 0;
}
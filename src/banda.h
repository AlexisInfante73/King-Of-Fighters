#ifndef BANDA_H
#define BANDA_H

#include <vector>
#include <iostream>
#include "personaje.h"

class Banda {
private:
    std::string nombreBanda;
    std::vector<Personaje*> integrantes;
    int velocidadBanda;
    int sinergiaTotal; // 🌟 NUEVO: Guardamos el bono total para usarlo en los gráficos

public:
    Banda(std::string _nombre) : nombreBanda(_nombre), velocidadBanda(0), sinergiaTotal(0) {}

    ~Banda() {
        // Buena práctica de POO: La banda limpia a sus propios integrantes de la memoria
        for (Personaje* p : integrantes) {
            delete p;
        }
        integrantes.clear();
    }

    void agregarIntegrante(Personaje* p) {
        if (integrantes.size() < 3) {
            integrantes.push_back(p);
        } else {
            std::cout << "¡La banda ya está llena! Máximo 3 integrantes." << std::endl;
        }
    }

    // LÓGICA DE SINERGIA (Mantiene tus espectaculares mecánicas intactas)
    void calcularSinergia() {
        int velocidadBaseTotal = 0;
        int bonoParodia = 0;
        int bonoAmabilidad = 0;

        for (Personaje* p : integrantes) {
            velocidadBaseTotal += p->getVelocidadBase();
            if (p->getEsCorazonPuro()) {
                bonoAmabilidad += 15; 
            }
        }

        if (integrantes.size() == 3) {
            std::string a1 = integrantes[0]->getAnimeOrigen();
            std::string a2 = integrantes[1]->getAnimeOrigen();
            std::string a3 = integrantes[2]->getAnimeOrigen();

            if (a1 == "Naranja Ninja" && a2 == "Una Pieza" && a3 == "Lejia") {
                bonoParodia = 50; 
            } else if (a1 == "Bochi la Piedra!" && a2 == "Bochi la Piedra!" && a3 == "Bochi la Piedra!") {
                bonoParodia = 90; 
            } else if (a1 == a2 && a2 == a3) {
                bonoParodia = 30; 
            }
        }

        sinergiaTotal = bonoParodia + bonoAmabilidad;
        velocidadBanda = velocidadBaseTotal + sinergiaTotal;
    }

    // 🎨 🌟 NUEVO MÉTODO GRÁFICO:
    // Posiciona y dibuja a los 3 integrantes uno al lado del otro en el escenario
    void dibujarBanda(float xInicial, float yFija) {
        float espaciado = 180.0f; // Distancia en pixeles entre cada músico
        for (size_t i = 0; i < integrantes.size(); ++i) {
            // Calculamos una posición X diferente para cada uno para que no se encimen
            integrantes[i]->setPosicion(xInicial + (i * espaciado), yFija);
            integrantes[i]->dibujar(); // Llama al dibujar() polimórfico de cada objeto hijo
        }
    }

    // Getters necesarios para el Main gráfico
    std::string getNombreBanda() const { return nombreBanda; }
    int getVelocidadBanda() const { return velocidadBanda; }
    int getSinergiaTotal() const { return sinergiaTotal; }
    const std::vector<Personaje*>& getIntegrantes() const { return integrantes; }
};

#endif
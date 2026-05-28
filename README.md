@startuml

skinparam style strictuml

abstract class Personaje {
    - nombre : String
    - energia : int
    - habilidadMusical : int
    + Personaje(nombre : String, energia : int, habilidad : int)
    + {abstract} ejecutarNota(tipoAccion : String, objetivo : Personaje) : void
    + reducirEnergia(cantidad : int) : void
}

class Vocalista {
    - carisma : int
    + ejecutarNota(tipoAccion : String, objetivo : Personaje) : void
    + cantarCantoGanador() : void
}

class Guitarrista {
    - velocidadDedos : int
    + ejecutarNota(tipoAccion : String, objetivo : Personaje) : void
    + soloDeGuitarra() : void
}

class Baterista {
    - precisionRitmo : int
    + ejecutarNota(tipoAccion : String, objetivo : Personaje) : void
    + mantenerElTempo() : void
}

class Banda {
    - nombreBanda : String
    - integrantes : Personaje[3]
    + estaEliminada() : boolean
}

class EscenarioCombate {
    - bandaJugador : Banda
    - bandaRival : Banda
    - sintonicaPublico : int
    + iniciarConcierto() : void
    + actualizarPublico() : void
}

Personaje <|-- Vocalista
Personaje <|-- Guitarrista
Personaje <|-- Baterista
Banda "1" *-- "3" Personaje : tiene
EscenarioCombate "1" *-- "2" Banda : se enfrentan

@endum()
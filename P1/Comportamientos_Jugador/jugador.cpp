#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

    Action accion = actIDLE;

    if (sensores.reset)
    {
        fil = col = 99;
        brujula = 0;
        ultimaAccion = actIDLE;
        girar_derecha = false;
        bien_situado = false;
        bikini = false;
        zapatillas = false;
        encontrarG = false;
        encontrarX = false;
        encontrarD = false;
        encontrarK = false;
        meChocoP = false;
        salirP = false;

        cont = 1;
        cont1 = 1;
        cont2 = 1;
        cont3 = 0;
        cont4 = 0;

        ultimaAccion = actIDLE;
    }

    switch (ultimaAccion)
    {
    case actFORWARD:
        switch (brujula)
        {
        case 0:
            fil--;
            break;
        case 1:
            col++;
            break;
        case 2:
            fil++;
            break;
        case 3:
            col--;
            break;
        }
        break;
    case actTURN_L:
        brujula = (brujula + 3) % 4;
        girar_derecha = (rand() % 2 == 0);
        break;
    case actTURN_R:
        brujula = (brujula + 1) % 4;
        girar_derecha = (rand() % 2 == 0);
        break;
    }

    // Activa sensores de orientación
    if (sensores.posF != -1 && !bien_situado)
    {
        fil = sensores.posF;
        col = sensores.posC;
        brujula = sensores.sentido;
        bien_situado = true;
    }

    if (bien_situado)
    {
        // mapaResultado[fil][col] = sensores.terreno[0];
        guardarVisitado(sensores);
    }

    // Gestion de bateria
    if (sensores.terreno[0] == 'A')
    {

        if (ultimaAccion == actFORWARD)
        {
            if (!bikini)
            {
                sensores.bateria = sensores.bateria - 200;
            }
            else
            {
                sensores.bateria = sensores.bateria - 10;
            }
        }
        else
        {
            if (!bikini)
            {
                sensores.bateria = sensores.bateria - 500;
            }
            else
            {
                sensores.bateria = sensores.bateria - 5;
            }
        }
    }

    if (sensores.terreno[0] == 'B')
    {
        if (ultimaAccion == actFORWARD)
        {
            if (!zapatillas)
            {
                sensores.bateria = sensores.bateria - 100;
            }
            else
            {
                sensores.bateria = sensores.bateria - 15;
            }
        }
        else
        {
            if (!zapatillas)
            {
                sensores.bateria = sensores.bateria - 3;
            }
            else
            {
                sensores.bateria = sensores.bateria - 1;
            }
        }
    }

    if (sensores.terreno[0] == 'T')
    {
        sensores.bateria = sensores.bateria - 2;
    }

    if (sensores.terreno[0] == 'P')
    {
        bien_situado = false;
    }

    if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S') && (sensores.superficie[2] == '_'))
    {
        accion = actFORWARD;
    }
    else if (!girar_derecha)
    {
        accion = actTURN_L;
    }
    else
    {
        accion = actTURN_R;
        meChocoP = true;
    }

    if (encontrarG && !bien_situado)
    {
        switch (ultimaAccion)
        {
        case actFORWARD:
            accion = actTURN_R;
            encontrarG = false;
            break;
        case actTURN_L:
            accion = actFORWARD;
            break;
        case actTURN_R:
            accion = actFORWARD;
            break;
        }
    }
    if (!bien_situado)
    {
        if (sensores.terreno[5] == 'G' || sensores.terreno[4] == 'G' || sensores.terreno[9] == 'G')
        {
            accion = actTURN_L;
            encontrarG = true;
        }
        if (sensores.terreno[7] == 'G' || sensores.terreno[13] == 'G')
        {
            accion = actTURN_R;
            encontrarG = true;
        }
        if (sensores.terreno[6] == 'G' || sensores.terreno[12] == 'G')
        {
            accion = actFORWARD;
        }
        if (sensores.terreno[2] == 'G')
        {
            accion = actFORWARD;
            encontrarG = false;
        }
    }
    else if (bien_situado && sensores.terreno[2] == 'G')
    {
        accion = actFORWARD;
    }

    // buscar zapatillas
    if (encontrarD)
    {
        switch (ultimaAccion)
        {
        case actFORWARD:
            accion = actTURN_R;
            encontrarD = false;
            break;
        case actTURN_L:
            accion = actFORWARD;
            break;
        case actTURN_R:
            accion = actFORWARD;
            break;
        }
    }
    if (!zapatillas)
    {
        if (sensores.terreno[5] == 'D' || sensores.terreno[4] == 'D' || sensores.terreno[9] == 'D' || sensores.terreno[11] == 'D' || sensores.terreno[1] == 'D')
        {
            accion = actTURN_L;
            encontrarD = true;
        }
        if (sensores.terreno[3] == 'D' || sensores.terreno[7] == 'D')
        {
            accion = actTURN_R;
            encontrarD = true;
        }
        if (sensores.terreno[6] == 'D' || sensores.terreno[12] == 'D')
        {
            accion = actFORWARD;
        }
        if (sensores.terreno[2] == 'D')
        {
            accion = actFORWARD;
            encontrarD = false;
            zapatillas = true;
        }
    }
    else if (zapatillas && sensores.terreno[2] == 'B')
    {
        accion = actFORWARD;
    }

    // buscar bikini
    if (encontrarK)
    {
        switch (ultimaAccion)
        {
        case actFORWARD:
            accion = actTURN_R;
            encontrarK = false;
            break;
        case actTURN_L:
            accion = actFORWARD;
            break;
        case actTURN_R:
            accion = actFORWARD;
            break;
        }
    }

    if (!bikini)
    {
        if (sensores.terreno[5] == 'K' || sensores.terreno[4] == 'K' || sensores.terreno[9] == 'K' || sensores.terreno[11] == 'K' || sensores.terreno[1] == 'K')
        {
            accion = actTURN_L;
            encontrarK = true;
        }
        if (sensores.terreno[7] == 'K' || sensores.terreno[13] == 'K')
        {
            accion = actTURN_R;
            encontrarK = true;
        }
        if (sensores.terreno[6] == 'K' || sensores.terreno[12] == 'K')
        {
            accion = actFORWARD;
        }
        if (sensores.terreno[2] == 'K')
        {
            accion = actFORWARD;
            encontrarK = false;
            bikini = true;
        }
    }
    else if (bikini && sensores.terreno[2] == 'A')
    {
        accion = actFORWARD;
    }

    if (sensores.terreno[2] == 'K')
    {
        accion = actFORWARD;
        encontrarK = false;
        bikini = true;
    }
    // buscar pila
    if (encontrarX)
    {
        switch (ultimaAccion)
        {
        case actFORWARD:
            accion = actTURN_R;
            encontrarK = false;
            break;
        case actTURN_L:
            accion = actFORWARD;
            break;
        case actTURN_R:
            accion = actFORWARD;
            break;
        }
    }
    if (sensores.terreno[5] == 'X' || sensores.terreno[4] == 'X' || sensores.terreno[9] == 'X')
    {
        accion = actTURN_L;
        encontrarK = true;
    }
    if (sensores.terreno[3] == 'X')
    {
        accion = actTURN_R;
        encontrarK = true;
    }
    if (sensores.terreno[6] == 'X' || sensores.terreno[12] == 'X')
    {
        accion = actFORWARD;
    }
    if (sensores.terreno[2] == 'X')
    {
        accion = actFORWARD;
        encontrarK = false;
        bikini = true;
    }

    if (sensores.terreno[0] == 'X')
    {
        if (sensores.bateria + 10 < 5000)
        {
            sensores.bateria = sensores.bateria + 10;
            accion = actTURN_R;
        }
        else
        {
            sensores.bateria = 5000;
        }
    }

    if (sensores.terreno[2] == 'P' && !meChocoP)
    {
        accion = actTURN_R;
        meChocoP = true;
    }
    else if (sensores.terreno[2] == 'P' && meChocoP)
    {
        accion = actTURN_L;
        meChocoP = false;
    }

    if (salirP)
    {
        accion = actTURN_R;
        salirP = false;
    }

    if (sensores.terreno[2] == 'T' && sensores.terreno[3] == 'T' && sensores.terreno[1] == 'P' && sensores.terreno[5] == 'P' && sensores.terreno[6] == 'P' && sensores.terreno[7] == 'P')
    {
        accion = actFORWARD;
        salirP = true;
    }

    if (sensores.superficie[2] == 'l' || sensores.superficie[2] == 'a')
    {
        accion = actTURN_R;
    }

    if (bien_situado && zapatillas && bikini)
    {
        if (sensores.terreno[3] == 'P')
        {
            cont3++;
        }
        if (cont3 == 8)
        {
            accion = actTURN_L;
            cont3 = 0;
        }
    }
    
    if (accion == actFORWARD)
    {
        cont4++;
        if (sensores.nivel == 4)
        {
            if (cont4 == 80)
            {
                accion = actTURN_R;
            }
            if (cont4 == 150)
            {
                accion = actTURN_L;
                cont4 = 0;
            }
        }
        else
        {
            if (cont4 == 100)
            {
                accion = actTURN_R;
            }
            if (cont4 == 200)
            {
                accion = actTURN_L;
                cont4 = 0;
            }
        }
    }

    if (bien_situado && zapatillas && bikini)
    {
        if (sensores.terreno[3] == 'P')
        {
            cont3++;
        }
        if (cont3 == 8)
        {
            accion = actTURN_L;
            cont3 = 0;
        }
    }

    ultimaAccion = accion;

    // Determinar el efecto de la ultima accion enviada
    return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
    return false;
}

void ComportamientoJugador::guardarVisitado(Sensores sensores)
{
    mapaResultado[fil][col] = sensores.terreno[0];

    if (brujula == 0)
    {
        mapaResultado[fil - 1][col - 1] = sensores.terreno[1];
        mapaResultado[fil - 1][col] = sensores.terreno[2];
        mapaResultado[fil - 1][col + 1] = sensores.terreno[3];

        mapaResultado[fil - 2][col - 2] = sensores.terreno[4];
        mapaResultado[fil - 2][col - 1] = sensores.terreno[5];
        mapaResultado[fil - 2][col] = sensores.terreno[6];
        mapaResultado[fil - 2][col + 1] = sensores.terreno[7];
        mapaResultado[fil - 2][col + 2] = sensores.terreno[8];

        mapaResultado[fil - 3][col - 3] = sensores.terreno[9];
        mapaResultado[fil - 3][col - 2] = sensores.terreno[10];
        mapaResultado[fil - 3][col - 1] = sensores.terreno[11];
        mapaResultado[fil - 3][col] = sensores.terreno[12];
        mapaResultado[fil - 3][col + 1] = sensores.terreno[13];
        mapaResultado[fil - 3][col + 2] = sensores.terreno[14];
        mapaResultado[fil - 3][col + 3] = sensores.terreno[15];
    }
    else if (brujula == 1)
    {
        mapaResultado[fil - 1][col + 1] = sensores.terreno[1];
        mapaResultado[fil][col + 1] = sensores.terreno[2];
        mapaResultado[fil + 1][col + 1] = sensores.terreno[3];

        mapaResultado[fil - 2][col + 2] = sensores.terreno[4];
        mapaResultado[fil - 1][col + 2] = sensores.terreno[5];
        mapaResultado[fil][col + 2] = sensores.terreno[6];
        mapaResultado[fil + 1][col + 2] = sensores.terreno[7];
        mapaResultado[fil + 2][col + 2] = sensores.terreno[8];

        mapaResultado[fil - 3][col + 3] = sensores.terreno[9];
        mapaResultado[fil - 2][col + 3] = sensores.terreno[10];
        mapaResultado[fil - 1][col + 3] = sensores.terreno[11];
        mapaResultado[fil][col + 3] = sensores.terreno[12];
        mapaResultado[fil + 1][col + 3] = sensores.terreno[13];
        mapaResultado[fil + 2][col + 3] = sensores.terreno[14];
        mapaResultado[fil + 3][col + 3] = sensores.terreno[15];
    }
    else if (brujula == 2)
    {
        mapaResultado[fil + 1][col + 1] = sensores.terreno[1];
        mapaResultado[fil + 1][col] = sensores.terreno[2];
        mapaResultado[fil + 1][col - 1] = sensores.terreno[3];

        mapaResultado[fil + 2][col + 2] = sensores.terreno[4];
        mapaResultado[fil + 2][col + 1] = sensores.terreno[5];
        mapaResultado[fil + 2][col] = sensores.terreno[6];
        mapaResultado[fil + 2][col - 1] = sensores.terreno[7];
        mapaResultado[fil + 2][col - 2] = sensores.terreno[8];

        mapaResultado[fil + 3][col + 3] = sensores.terreno[9];
        mapaResultado[fil + 3][col + 2] = sensores.terreno[10];
        mapaResultado[fil + 3][col + 1] = sensores.terreno[11];
        mapaResultado[fil + 3][col] = sensores.terreno[12];
        mapaResultado[fil + 3][col - 1] = sensores.terreno[13];
        mapaResultado[fil + 3][col - 2] = sensores.terreno[14];
        mapaResultado[fil + 3][col - 3] = sensores.terreno[15];
    }
    else if (brujula == 3)
    {
        mapaResultado[fil + 1][col - 1] = sensores.terreno[1];
        mapaResultado[fil][col - 1] = sensores.terreno[2];
        mapaResultado[fil - 1][col - 1] = sensores.terreno[3];

        mapaResultado[fil + 2][col - 2] = sensores.terreno[4];
        mapaResultado[fil + 1][col - 2] = sensores.terreno[5];
        mapaResultado[fil][col - 2] = sensores.terreno[6];
        mapaResultado[fil - 1][col - 2] = sensores.terreno[7];
        mapaResultado[fil - 2][col - 2] = sensores.terreno[8];

        mapaResultado[fil + 3][col - 3] = sensores.terreno[9];
        mapaResultado[fil + 2][col - 3] = sensores.terreno[10];
        mapaResultado[fil + 1][col - 3] = sensores.terreno[11];
        mapaResultado[fil][col - 3] = sensores.terreno[12];
        mapaResultado[fil - 1][col - 3] = sensores.terreno[13];
        mapaResultado[fil - 2][col - 3] = sensores.terreno[14];
        mapaResultado[fil - 3][col - 3] = sensores.terreno[15];
    }
}

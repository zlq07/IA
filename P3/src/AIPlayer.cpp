#include "AIPlayer.h"
#include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta

    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
    cout << "Valor Poda alfa beta: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

   /* // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
    case 1:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
        break;
    case 2:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
        break;
    }
    cout << "Valor Poda alfa beta: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    
    switch (id)
    {
    case 0:
        thinkAleatorio(c_piece, id_piece, dice);
        break;
    case 1:
        thinkAleatorioMasInteligente(c_piece, id_piece, dice);
        break;
    case 2:
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
        break;
    case 3:
        thinkMejorOpcion(c_piece, id_piece, dice);
        break;
    }
    */
}


double AIPlayer::MiValoracion1(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);
        
        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador += 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 25;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_jugador -= 10;
                }
                else if (estado.getBoard().getPiece(c, j).type == final_queue)
                {
                    puntuacion_jugador += 15;
                }
                else if (estado.isEatingMove())
                {
                    if (get<0>(estado.getLastAction()) == c)
                    {
                        puntuacion_jugador += 50;
                    }
                }
                puntuacion_jugador -= estado.distanceToGoal(c,j);
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente += 5;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 25;
                }
                else if (estado.getBoard().getPiece(c, j).type == home)
                {
                    puntuacion_oponente -= 10;
                }
                else if (estado.getBoard().getPiece(c, j).type == final_queue)
                {
                    puntuacion_oponente += 15;
                }
                else if (estado.isEatingMove())
                {
                    if (get<0>(estado.getLastAction()) == c)
                    {
                        puntuacion_oponente += 50;
                    }
                }
                puntuacion_oponente -= estado.distanceToGoal(c,j);
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const
{
    double mejor_hasta_momento;
    if (profundidad == profundidad_max || actual.gameOver()) // si hemos llegado al nodo final
    {
        return (*heuristic)(actual, jugador);
    }
    else
    {
        color last_c_piece = none; // El color de la ultima ficha que se movio
        int last_id_piece = -1;    // el id de la ultima ficha que se movio
        int last_dice = -1;        // el dado que se uso en el ultimo movimiento

        color pieza_hijo = none;
        int id_piece_hijo = -1;
        int dice_hijo = -1;

        Parchis siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);

        if (actual.getCurrentPlayerId() == jugador) // Nodo MAX
        {
            while (!(siguiente_hijo == actual))
            {
                mejor_hasta_momento = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad + 1, profundidad_max, pieza_hijo, id_piece_hijo, dice_hijo, alpha, beta, heuristic);
                if (mejor_hasta_momento > alpha)
                {
                    alpha = mejor_hasta_momento;
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                }
                if (alpha >= beta)
                { // poda beta
                    break;
                }
                siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);
            }
            return alpha;
        }
        else
        { // Nodo MIN
            while (!(siguiente_hijo == actual))
            {
                mejor_hasta_momento = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad + 1, profundidad_max, pieza_hijo, id_piece_hijo, dice_hijo, alpha, beta, heuristic);
                if (mejor_hasta_momento < beta)
                {
                    beta = mejor_hasta_momento;
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                }
                if (beta <= alpha)
                { // poda beta
                    break;
                }
                siguiente_hijo = actual.generateNextMove(last_c_piece, last_id_piece, last_dice);
            }
            return beta;
        }
    }
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
    // El color de ficha que se va a mover

    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // vector que almacenará los ids de las fichas que se pueden mover para el dado elegido
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);

    // en vez de elegir un dato al azar, miro primero cuáles tienen fichas que se puedan mover
    vector<int> current_dices_que_pueden_mover_ficha;
    for (int i = 0; i < current_dices.size(); i++)
    {
        // se obtiene el vector de fichas que se pueden mover para el dado elegido
        current_pieces = actual->getAvailablePieces(c_piece, current_dices[i]);

        // si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices[i]);
        }
    }
    // si no tengo ninun dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me permite no mover).
    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices[rand() % current_dices.size()];

        id_piece = SKIP_TURN;
    }

    // En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];

        // se obtiene e vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(c_piece, dice);

        // Muevo una ficha al azar de entre las que se pueden mover
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    // Elijo el dado haciendo lo mismo que el jugador anterior
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);
    // Trans llamar a esta funcion , ya tengo en dice el número de dado que quiero usar.
    // Ahora, en vez de mover una ficha al azar, voy a mover la que está más adelantada
    // (equivalente, la ás cercana a la meta).
    vector<int> current_pieces = actual->getAvailablePieces(c_piece, dice);

    int id_ficha_mas_adelantada = -1;
    int min_distancia_meta = 9999;
    for (int i = 0; i < current_pieces.size(); i++)
    {
        // distanceToGoal(color, id) devuelce la distancia a la meta de la fiche [id] del color que le indique.
        int distancia_meta = actual->distanceToGoal(c_piece, current_pieces[i]);
        if (distancia_meta < min_distancia_meta)
        {
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = current_pieces[i];
        }
    }

    // si no he encontrado ninguna ficha, paso turno
    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
    }
    // en caso contrario, moveré la ficha má adelantada
    else
    {
        id_piece = id_ficha_mas_adelantada;
    }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{
    // Vamos a mirar todo los posibles movimientos del jugador actual acediendo a los hijos del estado actual

    // generateNexMover va iterando sobre cada hijo. Le paso la accion del ultimo movimiento sobre
    //  el que he iterado y me devolverá el siguiente. Inicialmente, cuando aun no he hecho ningun
    //  movimiento, lo inicializo asi
    color last_c_piece = none; // El color de la ultima ficha que se movio
    int last_id_piece = -1;    // el id de la ultima ficha que se movio
    int last_dice = -1;        // el dado que se uso en el ultimo movimiento

    // Cuando ya he recorrido todo los hijos, la funcion devuelve el estado actual. De esta forma puedo saber
    // cuando paro de iterar
    Parchis siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);

    bool me_quedo_con_esta_accion = false;

    while (!(siguiente_hijo == *actual) && !me_quedo_con_esta_accion)
    {
        if (siguiente_hijo.isEatingMove() or                                            // si con este movimiento como ficha, o
            siguiente_hijo.isGoalMove() or                                              // si con este movimiento llego a la meta, o
            (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador) // sin con este movimiento gano la partida.
        )
        {
            // Me quedo con la accion actual (se almaceno en last_c_piece, last_id_piece, last_dice al llamar a generateNexMove)
            me_quedo_con_esta_accion = true;
        }
        else
        {
            // Genero el siguiente hijo
            siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);
        }
    }

    // Si he encontrado una accion qie me interesa, la guardo en las variables pasadas por referencia
    if (me_quedo_con_esta_accion)
    {
        c_piece = last_c_piece;
        id_piece = last_id_piece;
        dice = last_dice;
    }
    // Si no, muevo la ficha mas adelantada como antes
    else
    {
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }
}


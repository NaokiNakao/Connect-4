/*
    Conecta 4 (también conocido como 4 en Línea ó Connect Four) es un juego de mesa para dos jugadores
    distribuido por Hasbro, en el que se introducen fichas en un tablero vertical con el objetivo de alinear
    cuatro consecutivas de un mismo color. Fue creado en 1974 por Ned Strongin y Howard Wexler
    para Milton Bradley Company.

    El objetivo de Conecta 4 es alinear cuatro fichas sobre un tablero formado por seis filas y siete columnas.
    Cada jugador dispone de 21 fichas de un color (por lo general, rojas o amarillas). Por turnos, los jugadores
    deben introducir una ficha en la columna que prefieran (siempre que no esté llena) y ésta caerá a la
    posición más baja. Gana la partida el primero que consiga alinear cuatro fichas consecutivas de un mismo
    color en horizontal, vertical o diagonal. Si todas las columnas están llenas, pero nadie ha hecho una fila
    válida, hay empate.

    Se desea crear el juego “Conecta 4” permitiendo jugar contra la computadora. El jugador siempre toma el
    primer turno. La computadora evitará a toda costa que el jugador gane. El jugador podrá moverse en la
    primera fila del tablero y realizará su jugada presionando [ENTER]. Si una columna está llena, no se permite
    la selección. Si uno de los dos realiza una jugada que garantiza que en una próxima jugada tiene más de
    una oportunidad de ganar, se debe dar como ganador sin que se realice la siguiente jugada. Al terminar
    el juego el jugador indicará si desea volver a jugar o no.

    Modos de juego:
    1- Jugador Vs Jugador
    2- Jugador Vs Computadora

    Para terminar un juego o salir del programa, se puede pulsar la tecla [Esc].

    Se hará uso del algoritmo Minimax para la toma de decisión de jugada por parte de la computadora.
    Este algoritmo forma un árbol de decisión el cual busca la mejor jugada suponiendo que el contricante
    escogerá la peor para nosotros. De este forma, la computadora puede revisar distintos escenarios
    de juego. Al llegar a un nodo final (PC gana, jugador gana, empate o última capa disponible), se
    realiza una evaluación heurística de la posición.

    Para fines de optimización, se utilizará la poda alfa-beta; que permite obviar escenarios de
    juego no convenientes.

    Creado por : Naoki Nakao González
    Fecha      : 27/11/2020
    Mat/ID     : 1014-2124
    1750-ICC-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.c>

// directivas de preprocesador

#define MAXFIL           6
#define MAXCOL           7
#define COORD            1
#define POSINI          10
#define EMPTY          255
#define LINE             3
#define CONNECT        158
#define PROF             8 //indica la profundidad a la que llegará el árbol de decisión
#define INF      100000000 //se usará para la poda alfa-beta

#define LEFT           75
#define RIGHT          77
#define ENTER          13
#define ESC            27
#define TWO_PLAYERS    49
#define VS_COMP        50

#define LV      179
#define LH       95
#define GUION   196

#define CP1      RED
#define CP2      YELLOW
#define HUM      1
#define CPU      2

// funciones

void llenarMatriz(int [][MAXCOL]);
void jugadorVSjugador(int [][MAXCOL]);
void jugadorVSComputadora(int [][MAXCOL]);
void mostrarPosAct(int, int);
void insertarFicha(int [][MAXCOL], int, int);
void quitarFicha(int [][MAXCOL], int);
void setColor(int);
void colorDefault();
int mejorMovimiento(int [][MAXCOL], int, long, long, int);
int heuristic(int [][MAXCOL]);
int comb(int, int, int, int);
int count(int [], int);
int jugadaForzada(int [][MAXCOL]);
int dobleJugadaGanadora(int [][MAXCOL], int);
void mostrarTablero(int [][MAXCOL]);
void mostrarFichas(int [][MAXCOL], int, int);
int connect4(int [][MAXCOL], int);
int evaluar(int [][MAXCOL], int, int, int);
void indicarLineaGanadora(int [][MAXCOL], int [][MAXCOL], int);
void mostrarLineaGanadora(int [][MAXCOL], int, int, int);
void juegoGanado(int);
void juegoGanadoIA(int);
void juegoGanadoDobleOportunidad(int);
int empate(int [][MAXCOL]);
void salir();

int main()
{
    int matriz[MAXFIL][MAXCOL];
    char opc;

    while (1)
    {
        gotoxy(COORD*20, COORD);
        printf("CONNECT 4");
        gotoxy(COORD, COORD+2);
        printf("1- Jugador Vs Jugador");
        gotoxy(COORD, COORD+3);
        printf("2- Jugador Vs Computadora");
        gotoxy(COORD, COORD+4);
        printf("SALIR (Esc)");
        _setcursortype(100);
        gotoxy(COORD, COORD+6);
        printf("Seleccione una opci%cn : ", 162);

        do { //seleccionando modo de juego
            opc = getch();
        } while (opc != TWO_PLAYERS && opc != VS_COMP && opc != ESC);

        system("cls");

        llenarMatriz(matriz); //llenando la matriz de ceros

        if (opc == TWO_PLAYERS) //modo de juego : 2 jugadores
        {
            jugadorVSjugador(matriz);
        }

        else if (opc == VS_COMP) //modo de juego : jugador vs computadora
        {
            jugadorVSComputadora(matriz);
        }

        else //verificando si se desea finalizar el programa
        {
            gotoxy(COORD, COORD+2);
            printf("%cSeguro que desea salir del juego? S(i) o N(o) : ", 168);
            do {
                opc = toupper(getch());
            } while (opc != 'S' && opc != 'N');

            if (opc == 'S')
            {
                salir();
                break;
            }
            else
            {
                system("cls");
                continue;
            }
        }
    }

    return 0;
}

/*
    Función    : llenarMatriz
    Argumentos : int mat[][MAXCOL]
    Objetivo   : llenar la matriz con ceros (0)
    Retorno    : ---
*/
void llenarMatriz(int mat[][MAXCOL])
{
    for (int fil = 0; fil < MAXFIL; fil++)
    {
        for (int col = 0; col < MAXCOL; col++)
        {
            mat[fil][col] = 0;
        }
    }

    return;
}

/*
    Función    : jugadorVSjugador
    Argumentos : int mat[][MAXCOL]
    Objetivo   : control del juego para dos jugadores
    Retorno    : ---
*/
void jugadorVSjugador(int mat[][MAXCOL])
{
    int pos_act = 0, turno = 1;
    char tecla;

    mostrarTablero(mat); //mostrando el tablero

    while (1)
    {
        gotoxy(COORD*20, COORD);
        printf("CONNECT 4");

        _setcursortype(0);
        mostrarPosAct(pos_act, turno); //mostrando la posición actual del cursor

        if (connect4(mat, 1)) //verificando si el jugador 1 ganó
        {
            juegoGanado(1);
            break;
        }

        else if (connect4(mat, 2)) //verificando si el jugador 2 ganó
        {
            juegoGanado(2);
            break;
        }

        else if (empate(mat)) //verificando si hay empate
        {
            gotoxy(COORD, COORD+20);
            printf("EMPATE.\nPresione culquier tecla para continuar...");
            getch();
            system("cls");
            _setcursortype(100);
            break;
        }

        tecla = getch();

        if (tecla == LEFT && pos_act != 0) //movimiento del cursor a la izquierda
            pos_act--;

        else if (tecla == RIGHT && pos_act != MAXCOL - 1) //movimiento del cursor a la derecha
            pos_act++;

        else if (tecla == ENTER) //se activa al hacer una jugada
        {
            if ( !(mat[0][pos_act]) ) //se verifica si la columna está disponible para jugada
            {
                insertarFicha(mat, pos_act, turno);
                mostrarTablero(mat); //mostrando el tablero

                if (turno == 1)
                    turno++;
                else
                    turno--;
            }
        }

        else if (tecla == ESC) //verificando si se desea salir en medio del juego
        {
            _setcursortype(100);
            gotoxy(COORD, COORD+20);
            printf("%cSeguro que desea salir del juego? S(i) o N(o) : ", 168);
            do {
                tecla = toupper(getch());
            } while (tecla != 'S' && tecla != 'N');

            if (tecla == 'S')
            {
                system("cls");
                break;
            }
            else
            {
                system("cls");
                mostrarTablero(mat); //mostrando el tablero
                continue;
            }
        }
    }

    return;
}

/*
    Función    : jugadorVSComputadora
    Argumentos : int mat[][MAXCOL]
    Objetivo   : controla el juego de jugador contra computadora
    Retorno    : ---
*/
void jugadorVSComputadora(int mat[][MAXCOL])
{
    int pos_act = 0, fil, col, gameover = 0;
    char tecla;

    mostrarTablero(mat); //mostrando el tablero

    while (1)
    {
        gotoxy(COORD*20, COORD);
        printf("CONNECT 4");

        _setcursortype(0);
        mostrarPosAct(pos_act, HUM); //mostrando la posición actual del cursor

        if (gameover)
        {
            if (connect4(mat, CPU))
                juegoGanadoIA(CPU);

            else
                juegoGanadoIA(HUM);

            break;
        }

        else if (empate(mat)) //verificando si hay empate
        {
            gotoxy(COORD, COORD+20);
            printf("EMPATE.\nPresione culquier tecla para continuar...");
            getch();
            system("cls");
            _setcursortype(100);
            break;
        }

        tecla = getch();

        if (tecla == LEFT && pos_act != 0) //movimiento del cursor a la izquierda
            pos_act--;

        else if (tecla == RIGHT && pos_act != MAXCOL - 1) //movimiento del cursor a la derecha
            pos_act++;

        else if (tecla == ENTER) //se activa cuando el jugador desea insertar una pieza
        {
            if ( !(mat[0][pos_act]) && !gameover) //se verifica si la columna está disponible para jugada
            {
                insertarFicha(mat, pos_act, HUM); //el jugador inserta la ficha
                mostrarTablero(mat); //mostrando el tablero

                if (connect4(mat, HUM))
                    gameover++;

                if (dobleJugadaGanadora(mat, HUM) && !gameover) //el juego acaba si el jugador tiene más de una oportunidad de ganar
                {
                    juegoGanadoDobleOportunidad(HUM);
                    break;
                }

                if ( !gameover )
                {
                    for (col = 0; col < MAXCOL; col++) //la computadora realiza una jugada ganadora si la encuentra
                    {
                        if ( !(mat[0][col]) )
                        {
                            insertarFicha(mat, col, CPU);
                            if (connect4(mat, CPU))
                            {
                                gameover++;
                                mostrarTablero(mat); //mostrando el tablero
                                break;
                            }
                            else
                                quitarFicha(mat, col);
                        }
                    }
                }

                if (jugadaForzada(mat) && !gameover) //la computadora revisa si el jugador posee una jugada ganadora para poder bloquearla
                {
                    for (col = 0; col < MAXCOL; col++)
                    {
                        if ( !(mat[0][col]) )
                        {
                            insertarFicha(mat, col, HUM);

                            if (connect4(mat, HUM))
                            {
                                quitarFicha(mat, col);
                                insertarFicha(mat, col, CPU);
                                mostrarTablero(mat); //mostrando el tablero
                                break;
                            }
                            quitarFicha(mat, col);
                        }
                    }
                }

                else if ( !gameover ) //la computadora busca la mejor jugada si el jugador no ha ganado
                {
                    insertarFicha(mat, mejorMovimiento(mat, PROF, -INF, INF, 1), CPU);
                    mostrarTablero(mat); //mostrando el tablero
                }

                if (dobleJugadaGanadora(mat, CPU) && !gameover) //el juego acaba si la computadora tiene más de una oportunidad de ganar
                {
                    juegoGanadoDobleOportunidad(CPU);
                    break;
                }
            }
        }

        else if (tecla == ESC) //verificando si se desea salir en medio del juego
        {
            _setcursortype(100);
            gotoxy(COORD, COORD+20);
            printf("%cSeguro que desea salir del juego? S(i) o N(o) : ", 168);
            do {
                tecla = toupper(getch());
            } while (tecla != 'S' && tecla != 'N');

            if (tecla == 'S')
            {
                system("cls");
                break;
            }
            else
            {
                system("cls");
                mostrarTablero(mat); //mostrando el tablero
                continue;
            }
        }
    }

    return;
}

/*
    Función    : mostrarPosAct
    Argumentos : int marc, int color_jug
    Objetivo   : mostrar el movimiento del cursor con su respectivo color
    Retorno    : ---
*/
void mostrarPosAct(int marc, int color_jug)
{
    int x = POSINI*2+1, y = POSINI/2;

    if (color_jug == 1)
        color_jug = CP1;
    else
        color_jug = CP2;

    for (int val = 0; val < MAXCOL; val++)
    {
        colorDefault();

        if (val == marc)
            setColor(color_jug);

        gotoxy(x+val*4, y);
        printf("%c", EMPTY);
    }

    colorDefault();

    return;
}

/*
    Función    : insertarFicha
    Argumentos : int mat[][MAXCOL], int col, int n
    Objetivo   : insertar una ficha (1 o 2) en una columna
    Retorno    : ---
*/
void insertarFicha(int mat[][MAXCOL], int col, int n)
{
    int fil;

    for (fil = MAXFIL - 1; fil >= 0; fil--)
    {
        if (mat[fil][col] == 0)
        {
            mat[fil][col] = n;
            break;
        }
    }

    return;
}

/*
    Función    : quitarFicha
    Argumentos : int mat[][MAXCOL], int col
    Objetivo   : converitr a (0) el elemento indicado
    Retorno    : ---
*/
void quitarFicha(int mat[][MAXCOL], int col)
{
    int fil;

    for (fil = 0; fil < MAXFIL; fil++)
    {
        if (mat[fil][col] != 0)
        {
            mat[fil][col] = 0;
            break;
        }
    }

    return;
}

/*
    Función    : setColor
    Argumentos : int color
    Objetivo   : cambiar el color
    Retorno    : ---
*/
void setColor(int color)
{
    textbackground(color);

    return;
}

/*
    Función    : colorDefault
    Argumentos : ---
    Objetivo   : establecer el color por defecto (negro)
    Retorno    : ---
*/
void colorDefault()
{
    setColor(BLACK);

    return;
}

/*
    Función    : mejorMovimiento
    Argumentos : int mat[][MAXCOL], int prof, long alfa, long beta, int maxJug
    Objetivo   : encontrar el mejor movimiento para la computadora
    Retorno    : int mejor_mov
*/
int mejorMovimiento(int mat[][MAXCOL], int prof, long alfa, long beta, int maxJug)
{
    int val, col, act, mejor_mov;

    if (connect4(mat, HUM) || connect4(mat, CPU) || empate(mat)) //se comprueba si en el nodo hay un ganador o empate
    {
        if (connect4(mat, CPU))
            return INF;

        else if (connect4(mat, HUM))
            return -INF;

        else
            return 0;
    }

    if (prof == 0) //se comprueba si es un nodo terminal (última capa disponible)
    {
        return heuristic(mat);
    }

    if (maxJug) //se comprueba el mejor movimiento para el computador (maximiza puntos)
    {
        val = -INF;

        for (col = 0; col < MAXCOL; col++)
        {
            if ( !(mat[0][col]) )
            {
                insertarFicha(mat, col, CPU);
                act = mejorMovimiento(mat, prof-1, alfa, beta, 0);

                if (act > val)
                {
                    val = act;
                    mejor_mov = col;
                }

                quitarFicha(mat, col);

                if (val > alfa)
                        alfa = val;

                if (alfa >= beta)
                        break;
            }
        }
        return mejor_mov;
    }
    else //se comprueba el mejor movimiento para el juegador (minimiza puntos)
    {
        val = INF;

        for (col = 0; col < MAXCOL; col++)
        {
            if ( !(mat[0][col]) )
            {
                insertarFicha(mat, col, HUM);
                act = mejorMovimiento(mat, prof-1, alfa, beta, 1);

                if (act < val)
                {
                    val = act;
                    mejor_mov = col;
                }

                quitarFicha(mat, col);

                if (val < beta)
                    beta = val;

                if (alfa >= beta)
                    break;
            }
        }
        return mejor_mov;
    }
}

/*
    Función    : heuristic
    Argumentos : int mat[][MAXCOL]
    Objetivo   : evaluar las posiciones de juego más convenientes
    Retorno    : int puntos
*/
int heuristic(int arr [][MAXCOL])
{
    int fil, col, puntos = 0;

    //evaluando casillas centrales
    for (fil = MAXFIL - 1, col = 0; fil >= 0; fil--)
    {
        if (arr[fil][col+2] == CPU || arr[fil][col+4] == CPU)
            puntos += 3;

        if (arr[fil][col+3] == CPU)
            puntos += 4;
    }

    for (fil = MAXFIL - 2; fil >= 0; fil--){
        for (col = 1; col < MAXCOL - 1; col++)
        {
            if (arr[fil][col] == CPU && arr[fil][col-1] == HUM && arr[fil][col+1] == HUM)
                puntos += 100;
        }
    }

    //comprobando horizontales
    for (fil = 0; fil < MAXFIL; fil++)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            if (col - LINE >= 0)
            {
                puntos += comb(arr[fil][col], arr[fil][col-1], arr[fil][col-2], arr[fil][col-3]);
            }
            if (col + LINE < MAXCOL)
            {
                puntos += comb(arr[fil][col], arr[fil][col+1], arr[fil][col+2], arr[fil][col+3]) ;
            }
        }
    }

    //comprobando verticales
    for (fil = 0; fil < MAXFIL; fil++)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            if (fil - LINE >= 0)
            {
                puntos += comb(arr[fil][col], arr[fil-1][col], arr[fil-2][col], arr[fil-3][col]);
            }
            if (fil + LINE < MAXFIL)
            {
                puntos += comb(arr[fil][col], arr[fil+1][col], arr[fil+2][col], arr[fil+3][col]);
            }
        }
    }

    //comprobando diagonales
    for (fil = 0; fil < MAXFIL; fil++)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            if ( (fil - LINE >= 0) && (col - LINE >= 0) )
            {
                puntos += comb(arr[fil][col], arr[fil-1][col-1], arr[fil-2][col-2], arr[fil-3][col-3]);
            }

            if ( (fil + LINE < MAXFIL) && (col - LINE >=0) )
            {
                puntos += comb(arr[fil][col], arr[fil+1][col-1], arr[fil+2][col-2], arr[fil+3][col-3]);
            }

            if ( (fil - LINE >= 0) && (col + LINE < MAXCOL) )
            {
                puntos += comb(arr[fil][col], arr[fil-1][col+1], arr[fil-2][col+2], arr[fil-3][col+3]);
            }

            if ( (fil + LINE < MAXFIL) && (col + LINE < MAXCOL) )
            {
                puntos += comb(arr[fil][col], arr[fil+1][col+1], arr[fil+2][col+2], arr[fil+3][col+3]);
            }
        }
    }

    return puntos;
}

/*
    Función    : comb
    Argumentos : int a, int b, int c, int d
    Objetivo   : encontrar las diferentes combinaciones de piezas para evaluar posición
    Retorno    : int puntos
*/
int comb(int a, int b, int c, int d)
{
    int arr[] = {a, b, c, d}, puntos = 0;

    //combinaciones factibles para CPU

    if ( count(arr, CPU) == 3 && count(arr, 0) == 1 )
        puntos += 5;
    else if ( count(arr, CPU) == 2 && count(arr, 0) == 2 )
        puntos += 2;

    //combinaciones factibles para Jugador

    if ( count(arr, HUM) == 3 && count(arr, 0) == 1 )
        puntos -= 10;
    else if ( count(arr, HUM) == 2 && count(arr, 0) == 2 )
        puntos -= 4;

    return puntos;
}

/*
    Función    : count
    Argumentos : int arr[], int term
    Objetivo   : contar cuántas veces se repite "term" en "arr"
    Retorno    : int total
*/
int count(int arr[], int term)
{
    int index, total = 0;

    for (index = 0; index < 4; index++)
    {
        if (arr[index] == term)
            total++;
    }

    total++;
}

/*
    Función    : jugadaForzada
    Argumentos : int mat[][MAXCOL]
    Objetivo   : verificar si la computadora debe evitar que el jugador gane
    Retorno    : int flag
*/
int jugadaForzada(int mat[][MAXCOL])
{
    int col, flag = 0;

    for (col = 0; col < MAXCOL; col++)
    {
        if ( !(mat[0][col]) )
        {
            insertarFicha(mat, col, HUM);

            if (connect4(mat, HUM))
            {
                flag++;
            }

            quitarFicha(mat, col);
        }
    }

    return flag;
}

/*
    Función    : dobleJugadaGanadora
    Argumentos : int mat[][MAXCOL], int pieza
    Objetivo   : verificar si el jugador o la computadora tienen más de una oportunidad de ganar en un turno
    Retorno    : (1) si tiene más de una oportunidad; (0) en caso contrario o en caso de que el jugador tenga oportunidad de ganar
*/
int dobleJugadaGanadora(int mat[][MAXCOL], int pieza)
{
    int col, pieza_cont = HUM;

    if (pieza == HUM)
        pieza_cont = CPU;

    //se verifica si el contrario posee una jugada con la que gane en el siguiente movimiento
    for (col = 0; col < MAXCOL; col++)
    {
        if ( !(mat[0][col]) )
        {
            insertarFicha(mat, col, pieza_cont);

            if (connect4(mat, pieza_cont))
            {
                quitarFicha(mat, col);
                return 0;
            }
            else
                quitarFicha(mat, col);
        }
    }

    //se busca una doble jugada ganadora
    for (int i = 0; i < MAXCOL; i++)
    {
        if ( !(mat[0][i]) )
        {
            insertarFicha(mat, i, pieza);

            if (connect4(mat, pieza))
            {
                quitarFicha(mat, i);

                for (int j = 0; j < MAXCOL; j++)
                {
                    if ( !(mat[0][j]) && (j != i) )
                    {
                        insertarFicha(mat, j, pieza);

                        if (connect4(mat, pieza))
                        {
                            quitarFicha(mat, j);
                            return 1;
                        }
                        else
                            quitarFicha(mat, j);
                    }
                }
            }
            else
                quitarFicha(mat, i);
        }
    }

    return 0;
}

/*
    Función    : mostrarTablero
    Argumentos : int mat[][MAXCOL]
    Objetivo   : mostrar el tablero de juego
    Retorno    : ----
*/
void mostrarTablero(int mat[][MAXCOL])
{
    int posx = POSINI*2 - 1, posy = POSINI/2 + 1;

    gotoxy(posx, posy);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH, LH);
    gotoxy(posx, posy+1);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+2);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);
    gotoxy(posx, posy+3);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+4);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);
    gotoxy(posx, posy+5);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+6);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);
    gotoxy(posx, posy+7);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+8);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);
    gotoxy(posx, posy+9);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+10);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);
    gotoxy(posx, posy+11);
    printf("%c   %c   %c   %c   %c   %c   %c   %c", LV, LV, LV, LV, LV, LV, LV, LV);

    gotoxy(posx, posy+12);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c ", GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION, GUION);

    mostrarFichas(mat, posx, posy); //mostrando las jugadas hechas

    if (connect4(mat, 1) || connect4(mat, 2)) //mostrando conexión en caso de que haya
    {
        int matAux[MAXFIL][MAXCOL];
        llenarMatriz(matAux);

        if (connect4(mat, 1))
        {
            indicarLineaGanadora(mat, matAux, 1);
            mostrarLineaGanadora(matAux, posx, posy, CP1);
        }
        else
        {
            indicarLineaGanadora(mat, matAux, 2);
            mostrarLineaGanadora(matAux, posx, posy, CP2);
        }
    }

    return;
}

/*
    Función    : mostrarFichas
    Argumentos : int mat[][MAXCOL], int posx, int posy
    Objetivo   : mostrar el color de las fichas jugadas
    Retorno    : ---
*/
void mostrarFichas(int mat[][MAXCOL], int posx, int posy)
{
    posx += 2;
    posy++;

    int fil, col, aux = posx;

    for (fil = 0; fil < MAXFIL; fil++)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            gotoxy(posx, posy);

            if (mat[fil][col] == 1)
            {
                setColor(CP1);
                printf("%c", EMPTY);
            }
            else if (mat[fil][col] == 2)
            {
                setColor(CP2);
                printf("%c", EMPTY);
            }

            posx += 4;
            colorDefault();
        }
        posx = aux;
        posy += 2;
    }

    return;
}

/*
    Función    : connect4
    Argumentos : int arr[][MAXCOL], int n
    Objetivo   : comprobar si algún jugador conectó cuatro (4) piezas de manera vertical, horizontal o diagonal
    Retorno    : (1) si el jugador conectó cuatro piezas; (0) en caso contrario
*/
int connect4(int arr[][MAXCOL], int n)
{
    int fil, col;

    for (fil = MAXFIL-1; fil >= 0; fil--)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            if (evaluar(arr, fil, col, n))
                return 1;
        }
    }

    return 0;
}

/*
    Función    : evaluar
    Argumentos : int arr[][MAXCOL], int fil, int col, int n
    Objetivo   : identificar si una casilla específica de la matriz forma parte de cuatro piezas conectadas
    Retorno    : (1) si forma parte de cuatro piezas conectadas; (0) en caso contrario
*/
int evaluar(int arr[][MAXCOL], int fil, int col, int n)
{
    //casillas horizontales
    if (col - LINE >= 0)
    {
        if (arr[fil][col] == n && arr[fil][col-1] == n && arr[fil][col-2] == n && arr[fil][col-3] == n)
            return 1;
    }
    if (col + LINE < MAXCOL)
    {
        if (arr[fil][col] == n && arr[fil][col+1] == n && arr[fil][col+2] == n && arr[fil][col+3] == n)
            return 1;
    }

    //casillas verticales
    if (fil - LINE >= 0)
    {
        if (arr[fil][col] == n && arr[fil-1][col] == n && arr[fil-2][col] == n && arr[fil-3][col] == n)
            return 1;
    }
    if (fil + LINE < MAXFIL)
    {
        if (arr[fil][col] == n && arr[fil+1][col] == n && arr[fil+2][col] == n && arr[fil+3][col] == n)
            return 1;
    }

    //casillas diagonales
    if ( (fil - LINE >= 0) && (col - LINE >= 0) )
    {
        if (arr[fil-1][col-1] == n && arr[fil-2][col-2] == n && arr[fil-3][col-3] == n && arr[fil][col] == n)
            return 1;
    }

    if ( (fil + LINE < MAXFIL) && (col - LINE >=0) )
    {
        if (arr[fil+1][col-1] == n && arr[fil+2][col-2] == n && arr[fil+3][col-3] == n && arr[fil][col] == n)
            return 1;
    }

    if ( (fil - LINE >= 0) && (col + LINE < MAXCOL) )
    {
        if (arr[fil-1][col+1] == n && arr[fil-2][col+2] == n && arr[fil-3][col+3] == n && arr[fil][col] == n)
            return 1;
    }

    if ( (fil + LINE < MAXFIL) && (col + LINE < MAXCOL) )
    {
        if (arr[fil+1][col+1] == n && arr[fil+2][col+2] == n && arr[fil+3][col+3] == n && arr[fil][col] == n)
            return 1;
    }

    return 0;
}

/*
    Función    : indicarLineaGanadora
    Argumentos : int mat[][MAXCOL], int arr, int n
    Objetivo   : indicar las cuatro piezas conectadas, en caso de que hayan
    Retorno    : ---
*/
void indicarLineaGanadora(int mat[][MAXCOL], int arr[][MAXCOL], int n)
{
    int fil, col;

    for (fil = MAXFIL - 1; fil >= 0; fil--){
        for (col = 0; col < MAXCOL; col++)
        {
            if (col - LINE >= 0)
            {
                if (mat[fil][col] == n && mat[fil][col-1] == n && mat[fil][col-2] == n && mat[fil][col-3] == n)
                {
                    arr[fil][col] = 1, arr[fil][col-1] = 1, arr[fil][col-2] = 1, arr[fil][col-3] = 1;
                    return;
                }
            }
            if (col + LINE < MAXCOL)
            {
                if (mat[fil][col] == n && mat[fil][col+1] == n && mat[fil][col+2] == n && mat[fil][col+3] == n)
                {
                    arr[fil][col] = 1, arr[fil][col+1] = 1, arr[fil][col+2] = 1, arr[fil][col+3] = 1;
                    return;
                }
            }

            //casillas verticales
            if (fil - LINE >= 0)
            {
                if (mat[fil][col] == n && mat[fil-1][col] == n && mat[fil-2][col] == n && mat[fil-3][col] == n)
                {
                    arr[fil][col] = 1, arr[fil-1][col] = 1, arr[fil-2][col] = 1, arr[fil-3][col] = 1;
                    return;
                }
            }
            if (fil + LINE < MAXFIL)
            {
                if (mat[fil][col] == n && mat[fil+1][col] == n && mat[fil+2][col] == n && mat[fil+3][col] == n)
                {
                    arr[fil][col] = 1, arr[fil+1][col] = 1, arr[fil+2][col] = 1, arr[fil+3][col] = 1;
                    return;
                }
            }

            //casillas diagonales
            if ( (fil - LINE >= 0) && (col - LINE >= 0) )
            {
                if (mat[fil-1][col-1] == n && mat[fil-2][col-2] == n && mat[fil-3][col-3] == n && mat[fil][col] == n)
                {
                    arr[fil-1][col-1] = 1, arr[fil-2][col-2] = 1, arr[fil-3][col-3] = 1, arr[fil][col] = 1;
                    return;
                }
            }

            if ( (fil + LINE < MAXFIL) && (col - LINE >=0) )
            {
                if (mat[fil+1][col-1] == n && mat[fil+2][col-2] == n && mat[fil+3][col-3] == n && mat[fil][col] == n)
                {
                    arr[fil+1][col-1] = 1, arr[fil+2][col-2] = 1, arr[fil+3][col-3] = 1, arr[fil][col] = 1;
                    return;
                }
            }

            if ( (fil - LINE >= 0) && (col + LINE < MAXCOL) )
            {
                if (mat[fil-1][col+1] == n && mat[fil-2][col+2] == n && mat[fil-3][col+3] == n && mat[fil][col] == n)
                {
                    arr[fil-1][col+1] = 1, arr[fil-2][col+2] = 1, arr[fil-3][col+3] = 1, arr[fil][col] = 1;
                    return;
                }
            }

            if ( (fil + LINE < MAXFIL) && (col + LINE < MAXCOL) )
            {
                if (mat[fil+1][col+1] == n && mat[fil+2][col+2] == n && mat[fil+3][col+3] == n && mat[fil][col] == n)
                {
                    arr[fil+1][col+1] = 1, arr[fil+2][col+2] = 1, arr[fil+3][col+3] = 1, arr[fil][col] = 1;
                    return;
                }
            }
        }
    }

    return;
}

/*
    Función    : mostrarLineaGanadora
    Argumentos : int mat[][MAXCOL], int posx, int posy, int color
    Objetivo   : marcar la línea ganadora con "x"
    Retorno    : ---
*/
void mostrarLineaGanadora(int mat[][MAXCOL], int posx, int posy, int color)
{
    posx += 2;
    posy++;

    int fil, col, aux = posx;

    setColor(color);
    textcolor(BLUE);

    for (fil = 0; fil < MAXFIL; fil++)
    {
        for (col = 0; col < MAXCOL; col++)
        {
            gotoxy(posx, posy);

            if (mat[fil][col] == 1)
                printf("%c", CONNECT);

            posx += 4;
        }
        posx = aux;
        posy += 2;
    }

    colorDefault();
    textcolor(LIGHTGRAY);

    return;
}

/*
    Función    : juegoGanado
    Argumentos : int jugador
    Objetivo   : indicar ganador (2 jugadores)
    Retorno    : ---
*/
void juegoGanado(int jugador)
{
    gotoxy(COORD, COORD+20);
    printf("JUGADOR %d HA GANADO!!!\nPresione cualquier tecla para continuar...", jugador);
    getch();
    system("cls");

    return;
}

/*
    Función    : juegoGanadoIA
    Argumentos : int who
    Objetivo   : indicar ganador (jugador Vs computadora)
    Retorno    : ---
*/
void juegoGanadoIA(int who)
{
    gotoxy(COORD, COORD+20);

    if (who == HUM)
    {
        printf("FELICIDADES!!! HA GANADO\nPresione cualquier tecla para continuar...");
    }
    else
    {
        printf("EL COMPUTADOR HA GANADO\nPresione cualquier tecla para continuar...");
    }

    getch();
    system("cls");

    return;
}

/*
    Función    : juegoGanadoDobleOportunidad
    Argumentos : int who
    Objetivo   : indicar que ganó ya que tiene más de una oportunidad para hacerlo
    Retorno    : ---
*/
void juegoGanadoDobleOportunidad(int who)
{
    gotoxy(COORD, COORD+19);

    if (who == HUM)
    {
        printf("FELICIDADES!!! HA GANADO POR DOBLE OPORTUNIDAD\nPresione cualquier tecla para continuar...");
    }
    else
    {
        printf("EL COMPUTADOR HA GANADO POR DOBLE OPORTUNIDAD\nPresione cualquier tecla para continuar...");
    }

    getch();
    system("cls");

    return;
}

/*
    Función    : empate
    Argumentos : int mat[][MAXCOL]
    Objetivo   : verificar si queda al menos un elmento en el arreglo bidimensional que sea 0
    Retorno    : (1) si no queda al menos un cero; (0) en caso contrario
*/
int empate(int mat[][MAXCOL])
{
    int fil, col;

    for (fil = 0, col = 0; col < MAXCOL; col++)
    {
        if (mat[fil][col] == 0)
            return 0;
    }

    return 1;
}

/*
    Función    : salir
    Argumentos : ---
    Objetivo   : terminar el programa
    Retorno    : ---
*/
void salir()
{
    system("cls");
    gotoxy(COORD*20, COORD);
    printf("CONNECT 4");
    gotoxy(COORD, COORD+2);
    printf("%cGRACIAS POR JUGAR!", 173);

    return;
}







































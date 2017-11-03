#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>

#include "golosa.hpp"

using namespace std;

// TODO: esto de abajo es bastante rígido, porque depende del 'pwd'
#define BLUE_LOG_PATH "../log/player_communicator_azul.log"
#define RED_LOG_PATH "../log/player_communicator_rojo.log"

int contar_victorias(const char* color){
    /* 'color' debe ser "azul" o "rojo" */
    ifstream player_log;
    if (strcmp(color,"azul") == 0) {
        player_log.open(BLUE_LOG_PATH);
    }else if(strcmp(color, "rojo") == 0){
        player_log.open(RED_LOG_PATH);
    }else{
        cerr << "Color inválido" << endl;
        exit(1);
    }
    string palabra;
    int res = 0;
    while (player_log >> palabra) {
        if (palabra == "ganaste")
            res++;
    }
    return res;
}

string to_argv(vector<float> pesos){
    string s;
    for (uint i = 0; i < pesos.size(); i++) {
        s += std::to_string(pesos[i]);
        if (i < pesos.size()-1) s += " ";
    }
    return s;
}


int play_with_golosa(uint N, uint M, uint C, uint P, vector<float> pesos, uint games, string rival_exec, bool me_first, string root_loc) {
    /*
    PARAMETROS:
    - N, M, C, P:   Descripcion del tablero
    - pesos:        Los parametros que se le pasa a la funcion golosa
    - games:        Cantidad de partidos a Jugar
    - rival_exec:   Comando para ejecutar el codigo del oponente
    - me_first:     Indica si el goloso juega primero
    */

    // python2 c_linea.py —blue_player ai/golosa -1 1 1 1 1 1 1 0 1 2 3 4 5 6 7 —red_player ai/minimax_ab -1 1 1 1 1 1 1 0 1 2 3 4 5 6 7 —first rojo —columns 4 —rows 4 —c 3
    assert(pesos.size() == golosa::cuantos_parametros(N,M,C));
    char command[1000];
    string color_first = me_first ? "rojo" : "azul";
    string params = to_argv(pesos);
    sprintf (command, "python %sc_linea.py --blue_player %s --red_player %sgolosa %s --first %s --iterations %d --columns %d --rows %d --c %d --p %d",
             root_loc.c_str(), rival_exec.c_str(), root_loc.c_str(), params.c_str(), color_first.c_str(), games, N, M, C, P);
    cout << command << endl;
    system(command);
    return contar_victorias("rojo");
}

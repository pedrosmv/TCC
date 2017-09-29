#ifndef DECISION_H
#define DECISION_H

#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

/* Parametros usados na tomada de decisão principal do programa */
struct parameters {
        int corGrama;
        int vento;
        int insolacao;
        int umidade;
        float coeficienteChuva;
        float resultadoAnterior;
};

/* Struct que representa um bloco do campo e a necessidade de agua dele */
struct block_result {
        int x;
        int y;
        int qtd_agua;
};

enum estado {INICIAL, NAOREGA, CALC, REGA100, REGA70, REGA40, FIM};

float calcula_coeficiente(int weather[], float rain[]);
float parse_weather();
float resultadoAnterior();
float formula(parameters p );
int get_vento();
int get_umidade();
int get_insolacao();
int get_cor(bool regado, float dif_cor);
int state_machine(parameters param_dia);
int get_resAnterior(int x, int y, ifstream &input);
void rega(int quantidade_agua);
void save_resAnterior(vector<block_result> resultados, ofstream &output);

// void get_parameters(parameters* p, bool regado, float coeficiente);

#endif

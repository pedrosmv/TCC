#ifndef DECISION_H
#define DECISION_H

#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

struct parameters {
        int corGrama;
        int vento;
        int insolacao;
        int umidade;
        float coeficienteChuva;
        float resultadoAnterior;
};

struct block_result {
        int x;
        int y;
        int qtd_agua;
};

enum estado {INICIAL, NAOREGA, CALC, REGA100, REGA70, REGA40, FIM};

float calcula_coeficiente(int weather[], float rain[]);
float parse_weather();
int get_vento();
int get_umidade();
int get_insolacao();
int get_cor(bool regado);
float resultadoAnterior();
// void get_parameters(parameters* p, bool regado, float coeficiente);
float formula(parameters p );
void rega(int quantidade_agua);
int state_machine(parameters param_dia);
void save_resAnterior(vector<block_result> resultados, ofstream &output);
int get_resAnterior(int x, int y, ifstream &input);
#endif

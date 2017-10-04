#include "decision.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>

/* Calcula o coeficiente de chuva baseado na previsão do tempo de três dias */
float calcula_coeficiente(int tempo[], float chuva[]){
        int i = 0;
        float coeficiente = 0;
        for (i=0; i<12; i++) {
                coeficiente = coeficiente + (tempo[i] * (1 -(chuva[i]/100)));
        }

        return coeficiente/39;
}

/* Funcao responsavel por obter da internet a velocidade do vento no dia atual */
float find_wind(){
        string stream;
        float vento;
        string resultado;
        system("ansiweather -l Campinas > t.txt");
        ifstream file("t.txt");
        stream.assign( (istreambuf_iterator<char>(file)),
                       (istreambuf_iterator<char>()));

        const string s = stream;
        std::string::const_iterator start, end;
        start = s.begin();
        end   = s.end();
        regex expr{"[0-9]?[0-9]?(\.[0-9][0-9]?)? m/s"};
        smatch match;

        while(regex_search(start, end,match, expr)) {
                resultado = match[0];
                start = match[0].second;
        }
        cout << resultado << endl;
        vento = stof(resultado);
        file.close();
        return vento = vento * 3.6;
}


/* Essa funcao e responsavel por captar a previsao do tempo e separar os dados
   em dois vetores */
float parse_tempo(){
        string stream;
        string s;
        const string parameter = s;
        int i=0;
        int j=0;
        int tempo_coeficiente[12];
        float chuva_porcentagem[12];
        float coeficiente;
        system("curl wttr.in/'Campinas' > tempo.txt");

        ifstream tempo("tempo.txt");
        chuva_porcentagem[0] = 0.0;
        /* Esse loop vai parsear o arquivo de previsao do tempo e para cada previsao
           ele vai atribuir um valor numerico.
           Sunny = 3
           Clear = 2
           Cloudy = 1
           Overcast = 1
           Rain = 0
           Snow = 0
           Esses valores sao coeficientes que vao ser usados para calcular um coeficiente
           unico que vai ser usado na decisao final */
        while ( i < 12 )
        {
                tempo >> stream;
                size_t sunny = stream.find("Sunny", 0);
                if(sunny != string::npos)
                {
                        tempo_coeficiente[j] = 3;
                        j++;
                }
                size_t clear = stream.find("Clear", 0);
                if(clear != string::npos)
                {
                        tempo_coeficiente[j] = 2;
                        j++;
                }
                size_t rain = stream.find("Rain", 0);
                if(rain != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                rain = stream.find("rain", 0);
                if(rain != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t cloudy = stream.find("Cloudy", 0);
                if(cloudy != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                cloudy = stream.find("cloudy", 0);
                if(cloudy != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                size_t snow = stream.find("Snow", 0);
                if(snow != string::npos)
                {
                        tempo_coeficiente[j] = 0;
                        j++;
                }
                size_t overcast = stream.find("Overcast", 0);
                if(overcast != string::npos)
                {
                        tempo_coeficiente[j] = 1;
                        j++;
                }
                size_t found = stream.find('%');
                if(found != string::npos)
                {
                        float aux;
                        aux = stof(stream);
                        chuva_porcentagem[i+1] = aux;
                        i++;
                }
        }

        return coeficiente = calcula_coeficiente(tempo_coeficiente, chuva_porcentagem);
}
/* Funcao responsavel por obter o valor da variavel que representa o vento */
int get_vento(){

        float vento;
        int resultado;

        vento = find_wind();


        if (vento <= 5) {
                resultado = 1;
        }
        else if (vento > 5 && vento <= 10) {
                resultado = 2;
        }
        else if (vento > 10 && vento <= 15) {
                resultado = 3;
        }
        else if (vento > 15 && vento <= 20) {
                resultado = 4;
        }
        else if (vento > 20 && vento <= 25) {
                resultado = 5;
        }
        else{
                resultado = 0;
        }

        return resultado;
}

/* Funcao responsavel por captar o valor que o sensor de umidade retorna */
int get_umidade(){
        srand (time(NULL));
        int umidade;

        umidade = rand() % 4 + 1;
        return umidade;
}

/* Funcao responsavel por captar o valor que o sensor de insolacao retorna */
int get_insolacao(){
        srand (time(NULL));
        int insolacao;

        insolacao = rand() % 4 + 1;
        return insolacao;
}

/* Funcao que usa a diferença de cores entre o bloco e o minimo aceitavel para
   ela ser saudavel e retorna um valor que e usado na tomada de decisao */
int get_cor(bool regado, float dif_cor){

        if(regado) {
                return 80*((dif_cor/142)); /* 155 é a diferença maxima entre a grama boa e grama ruim */
        }

        return 0;
}

/* Funcao que gera o valor dos parametros obtidos para tomar a decisao de regar
   ou nao o bloco */
float formula(parameters p ){
        float resultado;

        resultado = p.coeficienteChuva*((p.corGrama + ((p.vento*p.insolacao)/p.umidade)) - p.resultadoAnterior);
        if (resultado < 0) {
                resultado = 0;
        }
        return resultado;
}

/* Processo de tomada de decisao, e uma maquina de estados que considera o valor
   retornado pela formula para ver o quanto sera utilizado de agua */
int state_machine(parameters param_dia){
        enum estado estado_atual = INICIAL;
        float decisao;
        int qtd_agua;

        while(estado_atual != FIM) {

                switch(estado_atual) {
                case INICIAL:

                        decisao = 100 * param_dia.coeficienteChuva;

                        if (decisao > 10) {
                                estado_atual = CALC;
                        }
                        else{
                                estado_atual = NAOREGA;
                        }
                        break;

                case CALC:
                        decisao = formula(param_dia);
                        // salva resultado em arquivo
                        if(decisao > 70) {
                                estado_atual = REGA100;
                        }
                        else if (decisao > 50 && decisao < 70) {
                                estado_atual = REGA70;
                        }
                        else if (decisao > 10 && decisao < 50) {
                                estado_atual = REGA40;
                        }
                        else {
                                estado_atual = NAOREGA;
                        }
                        break;

                case REGA100:
                        rega(100);
                        qtd_agua = 100;
                        estado_atual = FIM;
                        break;

                case REGA70:
                        rega(70);
                        qtd_agua = 70;
                        estado_atual = FIM;
                        break;

                case REGA40:
                        rega(40);
                        qtd_agua = 40;
                        estado_atual = FIM;
                        break;

                case NAOREGA:
                        rega(0);
                        qtd_agua = 0;
                        estado_atual = FIM;
                        break;

                case FIM:
                        break;
                }
        }
        return qtd_agua;
}

/* Placeholder para funcao de verdade que vai ser responsavel por ativar o sistema
   de irrigacao */
void rega(int quantidade_agua){
        cout << "A area selecionada foi regada em " << quantidade_agua << "% " << endl;
}

/* Salva o resultado anterior num arquivo */
void save_resAnterior(vector<block_result> resultados, ofstream &output){
        vector<block_result>::iterator itr;

        output.open("resAnterior", ios::app);
        for (itr = resultados.begin(); itr != resultados.end(); itr++) {
                output << (*itr).x << " " << (*itr).y << " " << (*itr).qtd_agua << endl;
        }
        output << " ";
        output.close();
}

/* Resgata o resultado anterior do arquivo */
int get_resAnterior(int x, int y, ifstream &input){
        int res_x, res_y, qtd_agua;
        input.open("resAnterior");
        string line;
        while ( getline (input,line) )
        {
                istringstream resultado(line);
                resultado >> res_x >> res_y >> qtd_agua;
                if(res_x == x && res_y == y) {
                        input.close();
                        return qtd_agua;
                }
        }
        input.close();
        return 0;
}

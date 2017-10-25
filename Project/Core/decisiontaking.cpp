#include "decision.h"
#include "sprinkler.h"
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
                cout << coeficiente << endl;
                cout << "tempo: " << tempo[i] << endl;
                cout << "chuva: " << chuva[i] << endl;
                coeficiente = coeficiente + (tempo[i] * (1 -(chuva[i]/100)));
        }

        return coeficiente/39;
}

/* Funcao responsavel por obter da internet a velocidade do vento no dia atual */
float find_wind(){
        string stream;
        float vento;
        string resultado;
        system("ansiweather -l Fortaleza > vento.txt");
        ifstream file("vento.txt");
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
        system("curl wttr.in/'Fortaleza' > tempo.txt");

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
                size_t moderate = stream.find("Moderate", 0);
                if(moderate != string::npos) {
                        tempo_coeficiente[j] = 2;
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

        int vento;
        float resultado;

        resultado = find_wind();


        if (resultado <= 5) {
                vento = 1;
        }
        else if (resultado > 5 && resultado <= 10) {
                vento = 2;
        }
        else if (resultado > 10 && resultado <= 15) {
                vento = 3;
        }
        else if (resultado > 15 && resultado <= 20) {
                vento = 4;
        }
        else if (resultado > 20 && resultado <= 25) {
                vento = 5;
        }
        else{
                vento = 0;
        }

        return vento;
}

/* Funcao responsavel por captar o valor que o sensor de umidade retorna */
int get_umidade(){

        string stream;
        ifstream file;
        string aux;
        int umidade_valor;

        file.open("sensordata.txt");

        while (getline (file, stream)) {

                stringstream line(stream);
                size_t found = stream.find("Umidade", 0);
                if(found != string::npos)
                {
                        line >> aux >> umidade_valor;
                }

        }
        return ((umidade_valor*4)/1023);
}

/* Funcao responsavel por captar o valor que o sensor de insolacao retorna */
float get_insolacao(){

        string stream;
        ifstream file;
        string aux;
        float insolacao_valor;

        file.open("sensordata.txt");

        while (getline (file, stream)) {

                stringstream line(stream);
                size_t found = stream.find("Insolacao", 0);
                if(found != string::npos)
                {
                        line >> aux >> insolacao_valor;
                }

        }
        return insolacao_valor;
}

/* Funcao que usa a diferença de cores entre o bloco e o minimo aceitavel para
   ela ser saudavel e retorna um valor que e usado na tomada de decisao */
int get_cor(bool regado, float dif_cor){

        if(regado) {
                return 80*((dif_cor/142)); /* 142 é a diferença maxima entre a grama boa e grama ruim */
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
                        decisao = formula(param_dia);
                        estado_atual = CALC;
                case CALC:
                        if(decisao > 10) {
                                estado_atual = REGA;
                        }
                        else {
                                estado_atual = NAOREGA;
                        }
                        break;

                case REGA:
                        qtd_agua = decisao;
                        estado_atual = FIM;
                        break;

                case NAOREGA:
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

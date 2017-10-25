#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string>
#include <regex>

using namespace std;


int main(){
        system("ansiweather -l Campinas > temperatura");
        string stream;
        string temperatura;
        string cronjob;
        float temp;
        ifstream file("temperatura");
        stream.assign( (istreambuf_iterator<char>(file)),
                       (istreambuf_iterator<char>()));

        const string s = stream;

        std::string::const_iterator start, end;
        start = s.begin();
        end   = s.end();
        regex expr{"[0-9]?[0-9]? °C"};
        smatch match;

        while(regex_search(start, end,match, expr)) {
                temperatura = match[0];
                start = match[0].second;
        }

        temp = stof(temperatura);
        system("crontab -r");
        if (temp > 10) {
                system("(crontab -l ; echo \"00 00 * * * /home/pedrosmv/Documents/TCC/Project/Core/runhomecontrol.sh $1 >/dev/null 2>&1\") | crontab -");
        }
        else
                system("(crontab -l ; echo \"00 06 * * * /home/pedrosmv/Documents/TCC/Project/Core/runhomecontrol.sh $1 >/dev/null 2>&1\") | crontab -");
}

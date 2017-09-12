#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

using namespace std;

int main(){
  string stream;
  int i;
  int nclearday = 0;

  system("curl wttr.in/'Campinas' > weather.txt");
  ifstream weather("weather.txt");

  stream.assign( (istreambuf_iterator<char>(weather)),
                (istreambuf_iterator<char>()));

  size_t clearday = stream.find("Clear", 0);
  size_t clearnight = stream.find('clear-night');
  size_t rain = stream.find('night');
  size_t snow = stream.find('snow');
  size_t sleet = stream.find('sleet');
  size_t wind = stream.find('wind');
  size_t fog = stream.find('fog');
  size_t cloudy = stream.find('cloudy');
  size_t partlycloudyday = stream.find('partly-cloudy-day');
  size_t partlycloudynight = stream.find('partly-cloudy-night');
  size_t thunderstorm = stream.find('thunderstorm');

  while(clearday != string::npos){
    nclearday++;
    clearday = stream.find("Clear", clearday+1);

  }
  cout << nclearday;


  return 0;
}

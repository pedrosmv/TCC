#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <regex>

using namespace std;

float calculate_coefficient(int weather[], float rain[]){
  int i = 0;
  float coefficient = 0;
  for (i=0; i<12; i++){
    coefficient = coefficient + (weather[i] * (1 -(rain[i]/100)));
  }

  return coefficient/39;
}

void find_wind(){
  int i;
  string stream;

  ifstream file("weather.txt");
  stream.assign( (istreambuf_iterator<char>(file)),
                (istreambuf_iterator<char>()));

  const string s = stream;
  boost::regex expr{"([0-9]{1,2})-([0-9]{1,2})"};
  boost::smatch match;

  if(boost::regex_match(s, expr)){
    cout << "Nice";
  }

  for(i=0; i < 12; i++){
    if (boost::regex_search(s, match, expr))
        cout << "match: " << match[i] << '\n';
  }
  file.close();
}

int main(){
  string stream;
  string s;
  const string parameter = s;
  int i=0;
  int j=0;
  int nclear = 0;
  int nrain = 0;
  int ncloudy = 0;
  int nsunny = 0;
  int nsnow = 0;
  int novercast = 0;
  int weather_coefficient[12];
  float rain_percentage[12];
  float coefficient;
  system("curl wttr.in/'London' > weather.txt");



  find_wind();

  ifstream weather("weather.txt");
  rain_percentage[0] = 0.0;
  /* This loop will parse the file where the weather report is saved and for every
  condition it will assign a value. These values are:
    Sunny = 3
    Clear = 2
    Cloudy = 1
    Overcast = 1
    Rain = 0
    Snow = 0
  These values are coefficients that will be used in the decision whether the
  selected tile will be irrigated or not. Besides the coefficients, the rain prediction
  will be used as well to calculate a final coefficient for the given day. */
  while ( i < 12 )
  {
    weather >> stream;
    size_t sunny = stream.find("Sunny", 0);
    if(sunny != string::npos)
    {
      nsunny++;
      weather_coefficient[j] = 3;
      j++;
    }
    size_t clear = stream.find("Clear", 0);
    if(clear != string::npos)
    {
      nclear++;
      weather_coefficient[j] = 2;
      j++;
    }
    size_t rain = stream.find("Rain", 0);
    if(rain != string::npos)
    {
      nrain++;
      weather_coefficient[j] = 0;
      j++;
    }
    rain = stream.find("rain", 0);
    if(rain != string::npos)
    {
      nrain++;
      weather_coefficient[j] = 0;
      j++;
    }
    size_t cloudy = stream.find("Cloudy", 0);
    if(cloudy != string::npos)
    {
      ncloudy++;
      weather_coefficient[j] = 1;
      j++;
    }
    cloudy = stream.find("cloudy", 0);
    if(cloudy != string::npos)
    {
      ncloudy++;
      weather_coefficient[j] = 1;
      j++;
    }
    size_t snow = stream.find("Snow", 0);
    if(snow != string::npos)
    {
      nsnow++;
      weather_coefficient[j] = 0;
      j++;
    }
    size_t overcast = stream.find("Overcast", 0);
    if(overcast != string::npos)
    {
      novercast++;
      weather_coefficient[j] = 1;
      j++;
    }
    size_t found = stream.find('%');
    if(found != string::npos)
    {
      float aux;
      aux = stof(stream);
      rain_percentage[i+1] = aux;
      i++;
    }
  }
  coefficient = calculate_coefficient(weather_coefficient, rain_percentage);

  weather.close();
  return 0;
}

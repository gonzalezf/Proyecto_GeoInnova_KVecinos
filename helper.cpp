#include "helper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


bool Helper::ArchivoExiste(const std::string& name){
    ifstream f(name.c_str());
    return f.good();
}

void Helper::ImprimirTiempoTranscurrido(double num){
    int hor,minu,seg;
    hor=(int)(num / 3600);
    minu=(int)((num - hor * 3600) / 60);
    seg=num - (hor * 3600 + minu * 60);
    cout<<"Tiempo Trancurrido: "<<hor<<"h "<<minu<<"m "<<seg<<"s"<<endl;
}

std::vector<std::vector<float>>  loadFile(std::string fileName, int maxVar){
  std::vector<std::vector<float>> data;
  float value;
  std::vector<float> point;
  int numVar;
  std::ifstream inFile(fileName.c_str());
  std::string line;
  //leer el título
  std::getline(inFile, line);
  //leer la cantidad de variables
  std::getline(inFile, line);
  std::istringstream iss (line);
  iss>> numVar;
  //leer el nombre de las variables
  for (int i = 0; i < numVar; i++) {
    std::getline(inFile, line);
  }
  //setear la cantidad de variables a usar
  if(maxVar==0)
    maxVar=numVar;
  //leer cada dato
  while (std::getline(inFile, line)) {
    std::istringstream iss (line);
    for (int i = 0; i < maxVar; i++) {
      iss>> value;
      point.push_back(value);
    }
    data.push_back(point);
    point.clear();
  }
  return data;
}
/*
int main (){
  std::vector<std::vector<float> > data;
  std::string fileName="../juraset.asc";
  //std::cout << "Ingrese o ruta del archivo: ";
  //std::cin >> fileName;
  data=loadFile(fileName,0);
  for (size_t  i = 0; i < data.size(); i++) {
    for (size_t j = 0; j < data[0].size(); j++) {
      std::cout << data[i][j]<<" ";
    }
    std::cout << std::endl;
  }
  return 0;
}
*/

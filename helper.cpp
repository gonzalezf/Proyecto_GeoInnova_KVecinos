#include "helper.h"


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
#include <iostream>
#include <fstream>
#include <list>
#include "utilidades.h"

using namespace std;

// estructura de un hecho
struct Hecho {
    string nombre; // nombre del hecho
    float factorCerteza; // factor de certeza del hecho
};

void leerHechos(const string& rutaBH, list<Hecho>& BH, string& meta);



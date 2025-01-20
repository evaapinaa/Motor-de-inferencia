#include <iostream>
#include <fstream>
#include <list>
#include "utilidades.h"

// Constantes globales para poder identificar el tipo de regla con mayor legibilidad
// en caso de que no sea ni conjuncion ni disyuncion, en el SBR-FC.cpp se condidera "0"
const int CONJUNCION = 1;
const int DISYUNCION = 2;


using namespace std;

// estructura de una regla
struct Regla {
    int id; // numero de regla
    string antecedentes;
    string consecuentes;
    int tipo; // CONJUNCION o DISYUNCION
    float factorCerteza;
};

void leerReglas(const string& rutaBC, list<Regla>& BC);


#ifndef LOGICA_INFERENCIA_H
#define LOGICA_INFERENCIA_H

#include "manejadorHechos.h"
#include "manejadorReglas.h"
#include <cmath>
#include <vector>
#include <filesystem>

using namespace std;

list<Regla> Equiparar(const list<Regla>& BC, const string& meta);
bool Contenida(const string& meta, const list<Hecho>& BH);
list<string> ExtraerAntecedentes(const Regla& R);
float calcularFC(float f1, float f2, int caso, int tipo);
Regla Resolver(list<Regla>& CC);
bool Verificar(const string& meta, list<Regla>& BC, list<Hecho>& BH, ostream& log);
bool EncaminamientoHaciaAtras(string& meta, list<Regla>& BC, list<Hecho>& BH, ostream& log);

#endif
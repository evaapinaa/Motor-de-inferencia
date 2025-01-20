#include "utilidades.h"

using namespace std;

// elimina los espacios en blanco al principio y al final de una cadena para evitar errores de lectura,
// y para que el programa funcione correctamente
string limpiarEspacios(const string& str) {
    size_t inicio = 0;
    while (inicio < str.size() && str[inicio] == ' ') {
        inicio++;
    }

    size_t fin = str.size();
    while (fin > inicio && str[fin - 1] == ' ') {
        fin--;
    }

    return str.substr(inicio, fin - inicio);
}


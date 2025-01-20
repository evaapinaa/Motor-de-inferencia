#include "manejadorReglas.h"

using namespace std;

// Funcion que lee las reglas del archivo y las guarda en BC
void leerReglas(const string& rutaBC, list<Regla>& BC) {
    ifstream file(rutaBC);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << rutaBC << endl;
        return;
    }

    string linea;
    getline(file, linea);

    try {
        int numReglas = stoi(linea); // numero de reglas
        for (int i = 0; i < numReglas; i++) {
            getline(file, linea);

            size_t posSi = linea.find("Si");
            size_t posEntonces = linea.find("Entonces");
            size_t posFC = linea.find("FC=");

            if (posSi == string::npos || posEntonces == string::npos || posFC == string::npos) {
                cerr << "Regla mal formulada." << linea << endl;
                continue;
            }

            // Para cada regla se extraen los antecedentes, consecuentes y el factor de certeza
            string antecedenteStr = linea.substr(posSi + 3, posEntonces - (posSi + 3)); 
            antecedenteStr = limpiarEspacios(antecedenteStr); 

            size_t posComa = linea.rfind(",", posFC);
            if (posComa == string::npos) {
                cerr << "No se puso la coma antes de FC." << linea << endl;
                continue;
            }

            string consecuenteStr = linea.substr(posEntonces + 9, posComa - (posEntonces + 9));
            consecuenteStr = limpiarEspacios(consecuenteStr);

            float fc = stof(linea.substr(posFC + 3));

            // identificar el tipo de regla
            int tipo = 0;
            if (antecedenteStr.find(" y ") != string::npos) {
                tipo = CONJUNCION;
            } else if (antecedenteStr.find(" o ") != string::npos) {
                tipo = DISYUNCION;
            }

            // guardar la regla en la bc
            Regla regla;
            regla.id = i + 1; // numero de la regla
            regla.antecedentes = antecedenteStr;
            regla.consecuentes = consecuenteStr;
            regla.tipo = tipo;
            regla.factorCerteza = fc;

            BC.push_back(regla);
        }
    } catch (const invalid_argument& e) {
        cerr << "Numero no valido en la primera linea." << linea << endl;
    }

    file.close();
}
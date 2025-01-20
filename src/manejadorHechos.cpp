#include "manejadorHechos.h"

using namespace std;

// lee los hechos del archivo de hechos y los guarda en la lista BH
void leerHechos(const string& rutaBH, list<Hecho>& BH, string& meta) {
    ifstream file(rutaBH);
    if (!file.is_open()) {
        cerr << "Error al abrir el fichero." << rutaBH << endl;
        return;
    }

    string linea;
    getline(file, linea);

    try {
        int numHechos = stoi(linea); // el numero de hechos
        for (int i = 0; i < numHechos; i++) {
            /* para cada hecho, se lee el nombre y el factor de certeza, y se usa limpiarEspacios 
             para eliminar los espacios en blanco y asi, evitar errores de lectura */
            getline(file, linea);
            size_t posNombre = linea.find(",");
            size_t posFC = linea.find("FC=");
            if (posNombre != string::npos && posFC != string::npos) {
                Hecho hecho;
                hecho.nombre = limpiarEspacios(linea.substr(0, posNombre));
                hecho.factorCerteza = stof(linea.substr(posFC + 3));
                BH.push_back(hecho);
            } else {
                cerr << "Linea no valida." << linea << endl;
            }
        }

        getline(file, linea);
        getline(file, meta);  // lee el objetivo, que es la ultima linea
        meta = limpiarEspacios(meta); // limpia los espacios en blanco en caso de que los haya

    } catch (const invalid_argument& e) {
        cerr << "Numero no valido en la primera linea." << linea << endl;
    }

    file.close();
}

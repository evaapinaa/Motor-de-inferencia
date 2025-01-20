#include "SBR-FC.h"

using namespace std;

// listas de reglas y hechos
list<Regla> BC;
list<Hecho> BH;


// Equiparar las reglas con la meta para obtener las reglas que tienen la meta como consecuente
list<Regla> Equiparar(const list<Regla>& BC, const string& meta) {
    list<Regla> CC;
    for (const auto& regla : BC) {
        if (regla.consecuentes == meta) {
            CC.push_back(regla);
        }
    }
    return CC;
}

// Comprueba si un hecho está contenido en la base de hechos y devuelve true en caso de que esté
bool Contenida(const string& meta, const list<Hecho>& BH) {
    for (const auto& hecho : BH) {
        if (hecho.nombre == meta) {
            return true;
        }
    }
    return false;
}

/* Extrae los antecedentes de una regla y los devuelve en una lista. Hay que tener en cuenta que los antecedentes 
pueden estar separados por "y" o "o" por lo que se deben extraer de forma individual. Usamos la funcion aux limpiarEspacios
para eliminar los espacios en blanco al principio y al final de la cadena, y asi evitar posibles fallos. Por ello se
utiliza la funcion find para buscar los delimitadores "y" y "o" y extraer los antecedentes de forma correcta. */
list<string> ExtraerAntecedentes(const Regla& R) {
    list<string> antecedentes;
    string antecedentesStr = R.antecedentes;
    string delimitadores[] = {" y ", " o "};

    while (!antecedentesStr.empty()) {
        size_t minEnd = string::npos;
        string selectedDelim;

        for (const string& delim : delimitadores) {
            size_t end = antecedentesStr.find(delim);
            if (end != string::npos && end < minEnd) {
                minEnd = end;
                selectedDelim = delim;
            }
        }

        if (minEnd == string::npos) {
            antecedentes.push_back(limpiarEspacios(antecedentesStr));
            break;
        } else {
            antecedentes.push_back(limpiarEspacios(antecedentesStr.substr(0, minEnd)));
            antecedentesStr.erase(0, minEnd + selectedDelim.size());
        }
    }

    return antecedentes;
}

// Para calcular el factor de certeza de un hecho, tenemos los 3 casos vistos en las diapositivas.
float calcularFC(float f1, float f2, int caso, int tipo) {
    switch (caso) {
        case 1: // Caso 1: Combinación de antecedentes (es conjunción o disyuncion)
            if (tipo == CONJUNCION) {
                return min(f1, f2);
            } else if (tipo == DISYUNCION) {
                return max(f1, f2);
            } else {
                return f1;
            }
        case 2: // Caso 2: Adquisición incremental de evidencia
            if ((f1 >= 0.0f && f2 >= 0.0f)) { // Ambos mayores o iguales a 0
                return f1 + f2*(1 - f1);
            } else if ((f1 <= 0.0f && f2 <= 0.0f)) { // Ambos negativos o iguales a 0
                return f1 + f2*(1 + f1);
            } else { // Signos distintos
                return (f1 + f2) / (1 - min(fabs(f1), fabs(f2)));
            }
        case 3: // Caso 3: Encadenamiento de evidencia
            return f2 * max(0.0f, f1);
        default:
            return f1;
    }
}

// Devuelve la regla seleccionada de la lista de reglas y la elimina de la lista
Regla Resolver(list<Regla>& CC) {

    Regla reglaSeleccionada = CC.front();
    CC.pop_front();
    return reglaSeleccionada;
}

/* 
 * Verificar: implementa la logica del motor de inferencia hacia atras.
 * 
 * 1) Comprueba si la meta ya está en la base de hechos (BH). Si es así, devuelve true
 * 
 * 2)Si no está en BH, busca reglas en la base de conocimiento (BC) que tengan la meta como consecuente (con Equiparar).
 *    Si no encuentra ninguna regla, devuelve false (no se puede inferir).
 * 
 * 3) Para cada regla encontrada (CC), extrae los antecedentes de la regla (ExtraerAntecedentes).
 *    Si algún antecedente no está en BH y no se puede inferir recursivamente,descarta esta regla.
 *    En caso contrario, almacena los factores de certeza de los antecedentes
 *    Si todos los antecedentes se verifican, es decir, están en BH o se pueden inferir, se procede a calcular el factor de certeza del consecuente.
 *        -Caso 1: Combina los fc de los antecedentes dependiendo del tipo de regla
 *        - Caso 3: Calcula el fc del consecuente aplicando el fc de la regla.
 * 
 * 4) Almacena el FC calculado en la Base de Hechos (BH):
 *      - Si el hecho ya existe, actualiza su fc.
 *      - Si no existe, lo agrega como un nuevo hecho.
 * 
 * 5) Por ultimo, unifica todos los factores de certeza obtenidos para la meta
 *    usando el caso 2, que sirve para combinar los fc de diferentes reglas.
 * 
 * 6) Devuelve true si el fcFinal de la meta es mayor que 0, indicando que
 *    se pudo inferir la meta.
 */

bool Verificar(const string& meta, list<Regla>& BC, list<Hecho>& BH, ostream& log) {
    if (Contenida(meta, BH)) {
        return true; // La meta está en la base de hechos
    }

    list<Regla> CC = Equiparar(BC, meta);
    if (CC.empty()) {
        return false; // No hay reglas con la meta como consecuente
    }

    vector<float> fcReglas; // almacenamos los factores de certeza de las reglas
    while (!CC.empty()) {
        Regla regla = Resolver(CC);

        // imprimir regla que ha sido seleccionada
        log << "R" << regla.id <<  ": Si " << regla.antecedentes << " Entonces " << regla.consecuentes << ", FC=" << regla.factorCerteza << "\n";

        list<string> antecedentes = ExtraerAntecedentes(regla);
        vector<float> fcAntedentes; // almacenamos los fc de los antecedentes
        vector<string> nombresAntecedentes(antecedentes.begin(), antecedentes.end()); // almacenamos los nombres de los antecedentes

        bool todosAntecedentes = true; // Tenemos que verificar si todos los antecedentes se pueden inferir o están en BH para aplicar la regla 
        for (auto& antecedente : antecedentes) {
            if (!Contenida(antecedente, BH) && !Verificar(antecedente, BC, BH, log)) {
                todosAntecedentes = false; // si no se puede inferir, entoncs no se puede aplicar la regla. Aqui esta la idea recursiva del encaminamiento hacia atras
                break;
            } else {
                for (const auto& hecho : BH) {
                    if (hecho.nombre == antecedente) {
                        fcAntedentes.push_back(hecho.factorCerteza);  // almacenar el fc del antecedente
                        break;
                    }
                }
            }
        }

        // Aplicar caso 1:
        float fcCombinado = 0.0f;
        if (!todosAntecedentes || fcAntedentes.empty()) {
            fcCombinado = 0.0f; // si no se cumple alguna de las condiciones, el fc es 0
        } else {
            // se aplica caso 1 segun el tipo de regla para combinar los fc de los antecedentesç
            fcCombinado = fcAntedentes[0];
            for (size_t i = 1; i < fcAntedentes.size(); ++i) {
                fcCombinado = calcularFC(fcCombinado, fcAntedentes[i], 1, regla.tipo);
            }
        }

        // imprimir el calculo que se ha hecho para el caso 1
        log << "\tCaso 1: ";
        for (size_t i = 0; i < fcAntedentes.size(); ++i) {
            log << nombresAntecedentes[i] << ", FC=" << fcAntedentes[i];
            if (i < fcAntedentes.size() - 1) {
                if (regla.tipo == CONJUNCION) log << " y ";
                else if (regla.tipo == DISYUNCION) log << " o ";
            }
        }
        log << ". Resultado=" << fcCombinado << "\n";

        // Aplicar caso 3 para calcular el factor de certeza del consecuente de la regla
        float fcConsecuente = calcularFC(fcCombinado, regla.factorCerteza, 3, 0);
        log << "\tCaso 3: " << regla.consecuentes << ", FC=" << fcCombinado << " * " << regla.factorCerteza << " = " << fcConsecuente << "\n"; // imprimir el calculo que se ha hecho para el caso 3

        // Actualizar BH con el nuevo hecho
        {
            bool encontrado = false;
            for (auto &h : BH) {
                if (h.nombre == regla.consecuentes) {
                    h.factorCerteza = fcConsecuente; // actualizar
                    encontrado = true;
                    break;
                }
            }
            if(!encontrado) {
                Hecho nuevoHecho;
                nuevoHecho.nombre = regla.consecuentes;
                nuevoHecho.factorCerteza = fcConsecuente;
                BH.push_back(nuevoHecho);
            }
        }

        fcReglas.push_back(fcConsecuente); // almacenar el fc del consecuente
    }

    // Aplicar caso 2: para unificar los fc de las reglas que tienen la meta como consecuente y obtener el fc final
    float fcFinal = 0.0f;
    for (const auto& fc : fcReglas) {
        fcFinal = calcularFC(fcFinal, fc, 2, 0);
    }
    log << "\tCaso 2: " << meta << ", FC=" << fcFinal << "\n"; // imprimir el calculo realizado para el caso 2

    // actializar con el fc final
    if (!fcReglas.empty()) {
        bool encontrado = false;
        for (auto &h : BH) {
            if (h.nombre == meta) {
                h.factorCerteza = fcFinal;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            Hecho nuevoHecho;
            nuevoHecho.nombre = meta;
            nuevoHecho.factorCerteza = fcFinal;
            BH.push_back(nuevoHecho);
        }
    }


    return fcFinal > 0; // devolver true o false segun el fc final
}

// Funcion principal de encaminamiento hacia atrss, que invoca a Verificar y devuelve true si la meta se puede inferir
bool EncaminamientoHaciaAtras(string& meta, list<Regla>& BC, list<Hecho>& BH, ostream& log) {
    if(Verificar(meta,BC,BH,log)) return true;
    return false;
}

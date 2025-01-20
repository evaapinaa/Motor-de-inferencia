#include "SBR-FC.h"

int main(int argc, char* argv[]) {

    // tenemos que vereificar que se proporcionen los argumentos necesarios
    if (argc < 3) {
        // si no se proporcionen suficientes argumentos, muestra un mensaje de error con el uso correcto
        cerr << "Uso: " << argv[0] << " <ruta_BC> <ruta_BH1> [<ruta_BH2> ...]\n";
        return 1;
    }

    // el primer argumento es la ruta del archivo de la base de conocimientos
    string rutaBC = argv[1];

    // para cada archivo de hechos, se realiza el proceso de inferencia y se guardan en archivos de texto
    for (int i = 2; i < argc; ++i) {
        string rutaBH = argv[i];
        list<Hecho> BH;
        list<Regla> BC;
        string meta;

        // leemos los fichero de hechos y reglas
        leerHechos(rutaBH, BH, meta);
        leerReglas(rutaBC, BC);

        std::filesystem::path rutaBCPath(rutaBC); // obtiene la ruta del archivo
        std::filesystem::path rutaBHPath(rutaBH); // obtiene la ruta del archivo
        string nombreConocimiento = rutaBCPath.stem().string(); // obtiene el nombre del archivo
        string nombreBase = rutaBHPath.stem().string(); // obtiene el nombre del archivo
        string rutaLog = "Resultado_" + nombreConocimiento + "_" + nombreBase + ".txt"; // crea el nombre del archivo Resultado_BC_BH.txt

        ofstream log(rutaLog);

        log << "Base de conocimientos: " << rutaBC << "\n";
        log << "Base de hechos: " << rutaBH << "\n";
        log << "Objetivo: " << meta << "\n\n";

        // invocar el motor de inferencia
        bool resultado = EncaminamientoHaciaAtras(meta, BC, BH, log);

         // imprimir el valor final del objetivo
        for (auto &h: BH) {
            if (h.nombre == meta) {
                log << "Objetivo: " << meta << ", FC=" << h.factorCerteza << "\n";
                break;
            }
        }

        log.close();

        // imprimir todo el desarrollo por pantalla tambien
        ifstream logRead(rutaLog);
        cout << logRead.rdbuf() << endl;

        // imprime si el objetivo se puede cumplir o no
        cout << "Resultado para " << rutaBH << ": ";
        if (resultado) {
            cout << "True";
        } else {
            cout << "False";
        }
        cout << endl;
    }

    return 0;
}

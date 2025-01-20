# SBR-FC

## Archivos del Proyecto

### main.cpp  
  punto de entrada del programa. Se encarga de:
  - Leer los argumentos dados en la consola.
  - Usa las funciones correspondientes para cargar esos ficheros y leerlos
  - Invoca la función de encadenamiento hacia atrás.
  - Escribe los resultados en archivos de salida (Resultado_<nombreBC_nombre-BH>.txt).
  - Adicionalmente, se ha puesto que se escriba el resultado del fichero por pantalla.

### SBR-FC.cpp / SBR-FC.h
  Contiene la implementación de las funciones del pseudocódigo de la transparencia, y sus respectivas modificaciones:
  - Funciones para equiparar reglas, ver si está contenida y extraer.
  - Verificación de hechos y evaluación de reglas, implementando la modificación de los fc.
  - Cálculo de fc combinando evidencias (calcularFC)
  - Función principal para ejecutar el encadenamiento hacia atrás, que llama a Verificar

### manejadorHechos.cpp / manejadorHechos.h
  Implementa lo necesario para gestionar la lectura de la base de hechos. Se encarga de:
  - Leer hechos desde un archivo externo, junto con sus factores de certeza.
  - Leer el objetivo especificado en el archivo.
  - Almacenar los hechos y el objetivo en estructuras de datos adecuadas.
  Por otro lado, en el .h también se define una estructura de datos para los hechos.

### manejadorReglas.cpp / manejadorReglas.h
  Implementa lo necesario para gestionar la lectura de la base de conocimientos. Se encarga de:
  - Leer las reglas desde un archivo externo.
  - Dividir las reglas en antecedentes y consecuentes.
  - Identificar el tipo de regla (si es conjunción o disyunción) y almacenar los factores de certeza.
  Por otro lado, en el .h también se define una estructura de datos para la Regla.

### utilidades.cpp / utilidades.h
  Para evitar problemas de cabeceras redefinidas con la función limpiarEspacios, se ha creado un modulo a parte para meterlo.
  Es una función util para evitar errores de lectura, por culpa de los espacios en blanco. Contiene:
   - limpiarEspacios: Elimina espacios en blanco al inicio y final de cadenas de texto.
  La verdad que este módulo se podría 
  haber evitado, poniendo en alguno de los manejadores, pero prefiero separarlo para tener el código organizado. Por otro lado, igual
  también se podría haber aprovechado más, implementando funciones para la salida del resultado. 
 

## Cómo Compilar
- Se explica en el archivo "ManualDeUso.pdf"
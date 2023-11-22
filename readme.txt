Proyecto2-SO

Integrantes
- Bruno Arce Rodriguez
- Luis Valenzuela Concha
- Emilio Ramos Montesino

Compilación y ejecución
Para compilar el proyecto se debe seguir la siguiente sintaxis:
$ g++ main.cpp && ./a.out [directorio] [umbral] [mutex/sem]

Un ejemplo de ejecución sería:
$ g++ main.cpp
$ ./a.out genomas 0.5 mutex


Evaluación experimental
Para la evaluación experimental se debe probar el ejecutable de la siguiente manera:
$ /usr/bin/time -v ./a.out genomas 0.5 mutex




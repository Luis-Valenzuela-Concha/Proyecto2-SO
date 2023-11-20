# Proyecto2-SO

## Descripción

## Integrantes
- *Bruno Arce Rodriguez*
- *Luis Valenzuela Concha*
- *Emilio Ramos Montesino*

## Compilacion y ejecucion
Para compilar el proyecto se debe seguir la siguiente sintaxis:
g++ main.cpp && ./a.out [directorio] [umbral] [mutex/sem]

Un ejemplo de ejecucion seria:
```
g++ main.cpp && ./a.out genomas 0.5 mutex
```

### Evaluacion experimental
Para la evaluacion experimental se debe probar el ejecutable de la siguiente manera:
```
/usr/bin/time -v ./a.out genomas 0.5 mutex
```



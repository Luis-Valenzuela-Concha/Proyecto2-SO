#include <bits/stdc++.h>
#include <string.h>
#include <filesystem>

using namespace std;

vector<string> copiarArchivoString(string &nombreArchivo){
    ifstream archivo(nombreArchivo);
    vector<string> vec;
    string linea;
    while (getline(archivo, linea)) {
        if (linea[0] == '>') continue;
        vec.push_back(linea);
    }
    archivo.close();
    return vec;
}

vector<string> obtenerArchivosEnDirectorio(const string& ruta) {
    vector<string> archivos;
    for (const auto& entrada : filesystem::directory_iterator(ruta)) {
        archivos.push_back(entrada.path().string());
    }
    return archivos;
}
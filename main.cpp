#include <bits/stdc++.h>
#include <string.h>
#include <filesystem>
#include <thread>
#include <semaphore>

using namespace std;

vector<string> copiarArchivoString(string nombreArchivo){
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

bool procesarGenoma(vector<string> genoma, float umbral) {
    int CG = 0;
    int total = 0;
    for (int i = 0; i < genoma.size(); i++) {
        string linea = genoma[i];
        for (int j = 0; j < linea.size(); j++) {
            if (linea[j] == 'C' || linea[j] == 'G') CG++;
            total++;
        }
    }
    return CG / (float)total >= umbral;
}

int main(int argc, char const *argv[]) {
    vector<string> nombreArchivos = obtenerArchivosEnDirectorio(argv[1]);
    vector<vector<string>> genomas;
    for(int i = 0; i < nombreArchivos.size(); i++) {
        genomas.push_back(copiarArchivoString(nombreArchivos[i]));
    }
    return 0;
}

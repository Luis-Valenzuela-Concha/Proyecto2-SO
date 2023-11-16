#include <bits/stdc++.h>
#include <string.h>

#include <fstream>
#include <iostream>

using namespace std;

vector<string> copiarArchivoString(string nombreArchivo) {
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

int main() {
    vector<string> vec = copiarArchivoString("GCF_000717965.1_ASM71796v1_genomic.fna");
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }
    return 0;
}

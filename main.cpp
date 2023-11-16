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

int main() {
    std::cout << "Hello, world!" << std::endl;
    return 0;
}

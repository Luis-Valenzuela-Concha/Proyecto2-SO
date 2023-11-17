#include <bits/stdc++.h>
#include <string.h>

#include <filesystem>
#include <pthread.h>
#include <semaphore>
#include <thread>

using namespace std;

pthread_mutex_t mutex_lock;
queue<string> cola_compartida;
float umbral;

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

bool procesarGenoma(vector<string> genoma, float umbral) {
    int CG = 0;
    int total = 0;
    for (int i = 0; i < genoma.size(); i++) {
        string linea = genoma[0];
        for (int j = 0; j < linea.size(); j++) {
            if (linea[j] == 'C' || linea[j] == 'G') CG++;
            total++;
        }
    }
    return CG / (float)total >= umbral;
}

void *procesarGenomaThread(void *arg) {
    string *archivo = static_cast<string*>(arg);
    vector<string> genoma = copiarArchivoString(*archivo);
    bool procesar = procesarGenoma(genoma,umbral);
    if(procesar){
        pthread_mutex_lock(&mutex_lock);
        cola_compartida.push(*archivo);
        pthread_mutex_unlock(&mutex_lock);
    }

    pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {
    //Extrae contenidos de archivos
    vector<string> nombreArchivos = obtenerArchivosEnDirectorio(argv[1]);

    int NUM_THREADS = nombreArchivos.size();
    pthread_t threads[NUM_THREADS];
    
    //Inicializar lock
    pthread_mutex_init(&mutex_lock, NULL);
    umbral = atof(argv[2]);
    //Crear threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, procesarGenomaThread, &nombreArchivos[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex_lock);
    
    cout << "Genomas que superan el umbral: " << endl;
    while(!cola_compartida.empty()){
        cout << cola_compartida.front() << endl;
        cola_compartida.pop();
    }

    return 0;
}

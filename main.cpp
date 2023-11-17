#include <bits/stdc++.h>
#include <string.h>

#include <filesystem>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

sem_t sem;
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

void *procesarGenoma_mutex_cv(void *arg) {
    string *archivo = static_cast<string*>(arg);
    vector<string> genoma = copiarArchivoString(*archivo);

    if(procesarGenoma(genoma,umbral)){
        pthread_mutex_lock(&mutex_lock);
        cola_compartida.push(*archivo);
        pthread_mutex_unlock(&mutex_lock);
    }

    pthread_exit(NULL);
}

void *procesarGenoma_semaforo(void *arg) {
    string *archivo = static_cast<string*>(arg);
    vector<string> genoma = copiarArchivoString(*archivo);

    if(procesarGenoma(genoma,umbral)){
        sem_wait(&sem);
        cola_compartida.push(*archivo);
        sem_post(&sem);
    }

    pthread_exit(NULL);
}

void printGenomas(){
    cout << "Genomas que superan el umbral: " << endl;
    while(!cola_compartida.empty()){
        cout << cola_compartida.front() << endl;
        cola_compartida.pop();
    }
}

int main(int argc, char const *argv[]) {
    //Extrae contenidos de archivos
    vector<string> nombreArchivos = obtenerArchivosEnDirectorio(argv[1]);
    umbral = atof(argv[2]);
    bool flag;
    if(argv[3] == "mutex") flag = true;
    else if(argv[3] == "sem") flag = false;
    else{
        cout << "Error en el tercer argumento" << endl;
        return 0;
    }

    if(argc != 4){
        cout << "Error en la cantidad de argumentos" << endl;
        return 0;
    }
    
    //Inicializar locks
    pthread_mutex_init(&mutex_lock, NULL);
    sem_init(&sem, 0, 1);

    //Crear hebras
    int NUM_THREADS = nombreArchivos.size();
    pthread_t threads[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++) {
        if(flag) pthread_create(&threads[i], NULL, procesarGenoma_mutex_cv, &nombreArchivos[i]);
        else pthread_create(&threads[i], NULL, procesarGenoma_semaforo, &nombreArchivos[i]);
    }

    //Esperar a que terminen las hebras
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex_lock);
    
    printGenomas();

    return 0;
}

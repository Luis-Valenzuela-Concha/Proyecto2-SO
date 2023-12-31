#include <bits/stdc++.h>
#include <string.h>
#include "procesarArchivos.cpp"

#include <filesystem>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

queue<string> cola_compartida;
float umbral;  

pthread_mutex_t mutex_lock;
pthread_cond_t c; 
sem_t sem; 

bool superaUmbral(vector<string> genoma, float umbral) {
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

void *superaUmbral_mutex_cv(void *arg) {
    string *archivo = static_cast<string*>(arg);
    vector<string> genoma = copiarArchivoString(*archivo);

    if(superaUmbral(genoma,umbral)){
        pthread_mutex_lock(&mutex_lock);
        cola_compartida.push(*archivo);
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&mutex_lock);
    }

    pthread_exit(NULL);
}

void *superaUmbral_semaforo(void *arg) {
    string *archivo = static_cast<string*>(arg);
    vector<string> genoma = copiarArchivoString(*archivo);

    if(superaUmbral(genoma,umbral)){
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
    if(argc != 4){
        cout << "Error en la cantidad de argumentos" << endl;
        return 0;
    }
    
    bool flag;
    string argv3 = argv[3];
    if(argv3 == "mutex") flag = true;
    else if(argv3 == "sem") flag = false;
    else{
        cout << "Error en el tercer argumento" << endl;
        return 0;
    }
    
    //Extrae contenidos de archivos
    vector<string> nombreArchivos = obtenerArchivosEnDirectorio(argv[1]);
    
    umbral = atof(argv[2]);

    //Inicializar locks
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_cond_init(&c, NULL);
    sem_init(&sem, 0, 1);

    //Crear hebras
    int NUM_THREADS = nombreArchivos.size();
    pthread_t threads[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++) {
        if(flag) pthread_create(&threads[i], NULL, superaUmbral_mutex_cv, &nombreArchivos[i]);
        else pthread_create(&threads[i], NULL, superaUmbral_semaforo, &nombreArchivos[i]);
    }

    //Esperar a que terminen las hebras
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex_lock);
    
    printGenomas();

    return 0;
}
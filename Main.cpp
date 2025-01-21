#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Arco;

class Ady {
public:
    Ady* sig;
    Arco* ref;
};

class Nodo {
public:
    char id;
    Nodo* sig;
    Ady* head;
    Nodo(char _id) {
        id = _id;
        sig = nullptr;
        head = nullptr;
    }
};

class Arco {
public:
    int peso;
    Nodo* first;
    Nodo* second;
    Arco* sig;
    Arco(int _peso, Nodo* _first, Nodo* _second) {
        peso = _peso;
        first = _first;
        second = _second;
        sig = nullptr;
    }
};

static Nodo* headNodo = nullptr;
static Arco* headArco = nullptr;
void crearNodos(int n){
    Nodo* aux = nullptr;
    if (n == 0) {
        char id = 65;
        Nodo* nuevoNodo = new Nodo(id);
        return;
    } else {
        for (int i = 0; i < n; i++) {
            char id = 65 + i;
            Nodo* nuevoNodo = new Nodo(id);
            if (headNodo == nullptr) {
                headNodo = nuevoNodo;
            } else {
                aux->sig = nuevoNodo;
            }
            aux = nuevoNodo;
        }
    }
}
void LeerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    getline(archivo, linea);
    int n = stoi(linea);
    crearNodos(n);
    
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
    }

    archivo.close();
}

int main() {
    string filename = "archivo.txt";
    LeerArchivo(filename);
    while(headNodo!=nullptr){
        cout<<headNodo->id<<",";
        headNodo = headNodo->sig;
    }

    return 0;
}

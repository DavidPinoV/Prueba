#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Nodo {
public:
    pair<char, int> dato;
    vector<Nodo*> sig;
};

class Arco {
public:
    int peso;
    Nodo* first;
    Nodo* second;
    Arco* sig;
};

class Ady {
public:
    Ady* sig;
    Arco* ref;
};

void LeerArchivo(const string& nombreArchivo) {
    ifstream archivo("archivo.txt");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        cout << "Procesando línea: " << linea << endl;
    }
    archivo.close();
}

int main() {
    LeerArchivo("datos.txt");

    return 0;
}

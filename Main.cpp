#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Arco;
class Nodo;

class Ady {
public:
    pair<int,Nodo*> principal;
    vector<Nodo*> sig;

};

class Nodo {
public:
    char id;
    Ady* head;
    Nodo(char _id) {
        id = _id;
        head = nullptr;
    }
};

static Nodo* headNodo = nullptr;
static Ady* headAdy = nullptr;

void crearNodos(int n) {
    Nodo* aux = nullptr;
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

Nodo* buscarNodos(int k,int n){
    Nodo* aux = headNodo;
    for (int i = 0; i < n; i++){
        if(aux==nullptr){return nullptr;}
        if(k==i){
            return aux;
        }
        aux = aux->sig;
    }    
    return nullptr;
}

void LeerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }
    string linea;    
    getline(archivo, linea);
    if(linea == "0"){cout<<"No hay Matriz Ady\n";return;}
    int n = stoi(linea);
    crearNodos(n);
 
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
                
    }

    archivo.close();
}

int main() {
    LeerArchivo("archivo.txt");
    while(headNodo!=nullptr){
        cout<<headNodo->id<<"\n";
        headNodo = headNodo->sig;
    }
    return 0;
}


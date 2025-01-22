#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>
using namespace std;

class Nodo;

class Ady {
public:
    vector<pair<int, Nodo*>> secundario;
    Ady() {}
    void agregarConectores(int peso, Nodo* nodo) {
        secundario.push_back({peso, nodo});
    }
};

class Nodo {
public:
    char id;
    Nodo* sig;
    Ady* head;
    Nodo(char _id) {
        id = _id;
        sig = nullptr;
        head = new Ady();
    }
};

class Arbol {
public:
    pair<char, int> nodo;
    vector<Arbol*> caminos;

    Arbol(pair<char, int> nodo) : nodo(nodo) {}

    void agregarCamino(Arbol* camino) {
        caminos.push_back(camino);
    }

    void imprimirRecorrido() {
        if(nodo.first==65){
            cout << nodo.first << " (" << 0 << ")";
        }
        else{
            cout << nodo.first << " (" << nodo.second << ")";
            
        }
        for (int i = 0; i<caminos.size();i++) {
            cout << " -> ";
            caminos[i]->imprimirRecorrido();
        }
        
    }
};

static Nodo* headNodo = nullptr;
static int n;

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

Nodo* buscarNodos(int k, int n) {
    Nodo* aux = headNodo;
    for (int i = 0; i < n; i++) {
        if (aux == nullptr) return nullptr;
        if (k == i) {
            return aux;
        }
        aux = aux->sig;
    }
    return nullptr;
}

void LeerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se leyo " << nombreArchivo << endl;
        return;
    }
    string linea;
    getline(archivo, linea);
    if (linea == "0") {
        cout << "No hay Matriz Ady\n";
        return;
    }
    n = stoi(linea);
    crearNodos(n);

    int i = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        int peso;
        int j = 0;
        while (ss >> peso) {
            if (ss.peek() == ',') ss.ignore();
            if (i != j && peso > 0) {
                Nodo* nodoInicio = buscarNodos(i, n);
                Nodo* nodoDestino = buscarNodos(j, n);
                nodoInicio->head->agregarConectores(peso, nodoDestino);
            }
            j++;
        }
        i++;
    }
    archivo.close();
}

void dijkstra(Nodo* nodoInicio, int n, vector<int>& v1, vector<int>& v2) {
    vector<bool> visitados(n, false);
    v1[nodoInicio->id - 65] = 0;

    for (int i = 0; i < n; i++) {
        int nodoActualIndex = -1;
        int minDistancia = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!visitados[j] && v1[j] < minDistancia) {
                minDistancia = v1[j];
                nodoActualIndex = j;
            }
        }

        if (nodoActualIndex == -1) break;

        visitados[nodoActualIndex] = true;
        Nodo* nodoActual = buscarNodos(nodoActualIndex, n);

        for (int i = 0; i < nodoActual->head->secundario.size(); ++i) {
            int peso = nodoActual->head->secundario[i].first;
            Nodo* vecino = nodoActual->head->secundario[i].second;
            int vecinoIndex = vecino->id - 'A';

            if (!visitados[vecinoIndex] && v1[nodoActualIndex] + peso < v1[vecinoIndex]) {
                v1[vecinoIndex] = v1[nodoActualIndex] + peso;
                v2[vecinoIndex] = nodoActualIndex;
            }
        }

    }
}

void reconstruirCamino(int destinoIndex, const vector<int>& v2, vector<char>& camino) {
    if (destinoIndex == -1) return;
    reconstruirCamino(v2[destinoIndex], v2, camino);
    camino.push_back(destinoIndex + 65);
}

void crearArbolImprimir(Nodo* nodoInicio, vector<int>& v1, int destinoIndex, vector<char>& camino);

void procesarRuta()
{
    if (headNodo != nullptr) {
        Nodo* nodoInicio = headNodo;
        vector<int> v1(n, INT_MAX);
        vector<int> v2(n, -1);

        dijkstra(nodoInicio, n, v1, v2);
        
        char destino;
        cout << "Ingrese el nodo que quieres llegar (Mayusculas): ";
        cin >> destino;

        int destinoIndex = destino - 65;
        if (destinoIndex >= 0 && destinoIndex < n) {
            if (v1[destinoIndex] == INT_MAX) {
                cout << "El nodo " << destino << " no conecta con A\n";
            } else {
                cout << destino << " " << v1[destinoIndex] << "\n";

                vector<char> camino;
                reconstruirCamino(destinoIndex, v2, camino);

                crearArbolImprimir(nodoInicio, v1, destinoIndex, camino);

                cout<<endl;
            }
        } else {
            cout << "El nodo ingresado no existe compa.\n";
        }
    }
}

void crearArbolImprimir(Nodo* nodoInicio, vector<int>& v1, int destinoIndex, vector<char>& camino) {
    Arbol* arbol = new Arbol({nodoInicio->id, 0});
    for (size_t i = 1; i < camino.size(); i++) {
        Arbol* subArbol = new Arbol({camino[i], v1[camino[i] - 65]});
        arbol->agregarCamino(subArbol);
    }
    arbol->imprimirRecorrido();
}


void imprimirTodosLosNodos() {
    Nodo* aux = headNodo;
    cout << "Todos los nodos: ";
    while (aux->sig != nullptr) {
        cout << aux->id << ", ";
        aux = aux->sig;
    }
    cout << aux->id << "\n";
}

int main() {
    LeerArchivo("archivo.txt");
    imprimirTodosLosNodos();
    procesarRuta();

    return 0;
}
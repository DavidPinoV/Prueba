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
    pair<char, int> nodo; // Nodo con su peso (caracter, peso acumulado)
    vector<Arbol*> caminos; // Caminos recorridos desde este nodo

    Arbol(pair<char, int> nodo) : nodo(nodo) {}

    void agregarCamino(Arbol* camino) {
        caminos.push_back(camino);
    }

    void imprimirRecorrido() {
        cout << nodo.first << " (" << nodo.second << ")";
        if (caminos.empty()) {
            cout << endl;
        } else {
            cout << " -> ";
            for (Arbol* camino : caminos) {
                camino->imprimirRecorrido();
            }
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

void LeerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
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
                if (nodoInicio && nodoDestino) {
                    nodoInicio->head->agregarConectores(peso, nodoDestino);
                }
            }
            j++;
        }
        i++;
    }
    archivo.close();
}

void dijkstra(Nodo* nodoInicio, int n, vector<int>& distancias, vector<int>& predecesores) {
    vector<bool> visitados(n, false);
    distancias[nodoInicio->id - 65] = 0;

    for (int i = 0; i < n; i++) {
        int nodoActualIndex = -1;
        int minDistancia = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!visitados[j] && distancias[j] < minDistancia) {
                minDistancia = distancias[j];
                nodoActualIndex = j;
            }
        }

        if (nodoActualIndex == -1) break;

        visitados[nodoActualIndex] = true;
        Nodo* nodoActual = buscarNodos(nodoActualIndex, n);

        for (const auto& arista : nodoActual->head->secundario) {
            Nodo* vecino = arista.second;
            int peso = arista.first;
            int vecinoIndex = vecino->id - 65;

            if (!visitados[vecinoIndex] && distancias[nodoActualIndex] + peso < distancias[vecinoIndex]) {
                distancias[vecinoIndex] = distancias[nodoActualIndex] + peso;
                predecesores[vecinoIndex] = nodoActualIndex;
            }
        }
    }
}

void reconstruirCamino(int destinoIndex, const vector<int>& predecesores, vector<char>& camino) {
    if (destinoIndex == -1) return;
    reconstruirCamino(predecesores[destinoIndex], predecesores, camino);
    camino.push_back(destinoIndex + 65);
}

void procesarRuta() {
    if (headNodo != nullptr) {
        Nodo* nodoInicio = headNodo;
        vector<int> distancias(n, INT_MAX);
        vector<int> predecesores(n, -1);

        dijkstra(nodoInicio, n, distancias, predecesores);
        
        char destino;
        cout << "Ingrese el nodo que quieres llegar (Mayusculas): ";
        cin >> destino;

        int destinoIndex = destino - 65;
        if (destinoIndex >= 0 && destinoIndex < n) {
            if (distancias[destinoIndex] == INT_MAX) {
                cout << "El nodo " << destino << " no conecta con A\n";
            } else {
                cout << destino << " " << distancias[destinoIndex] << "\n";

                vector<char> camino;
                reconstruirCamino(destinoIndex, predecesores, camino);

                cout << "Camino: ";
                for (int i = 0; i < camino.size(); i++) {
                    cout << camino[i];
                    if (i < camino.size() - 1) cout << "->";
                }
                cout << "\n";

                Arbol* arbol = new Arbol({nodoInicio->id, distancias[destinoIndex]});
                for (char c : camino) {
                    Arbol* subArbol = new Arbol({c, distancias[c - 65]});
                    arbol->agregarCamino(subArbol);
                }

                arbol->imprimirRecorrido();
            }
        } else {
            cout << "El nodo ingresado no es válido.\n";
        }
    }
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

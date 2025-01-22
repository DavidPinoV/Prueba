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
            if (ss.peek() == ',') ss.ignore(); // Ignorar comas
            if (i != j && peso > 0) { // Si hay un peso válido y no es la diagonal principal
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
    distancias.assign(n, INT_MAX);
    predecesores.assign(n, -1);

    distancias[nodoInicio->id - 65] = 0;

    for (int i = 0; i < n; i++) {
        int minDistancia = INT_MAX;
        int nodoActualIndex = -1;

        // Encontrar el nodo no visitado con menor distancia
        for (int j = 0; j < n; j++) {
            if (!visitados[j] && distancias[j] < minDistancia) {
                minDistancia = distancias[j];
                nodoActualIndex = j;
            }
        }

        if (nodoActualIndex == -1) break; // Todos los nodos alcanzables han sido visitados

        visitados[nodoActualIndex] = true;

        Nodo* nodoActual = buscarNodos(nodoActualIndex, n);
        for (const auto& arista : nodoActual->head->secundario) {
            Nodo* vecino = arista.second;
            int peso = arista.first;
            int vecinoIndex = vecino->id - 65;

            if (!visitados[vecinoIndex] &&
                distancias[nodoActualIndex] != INT_MAX &&
                distancias[nodoActualIndex] + peso < distancias[vecinoIndex]) {
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
        vector<int> distancias;
        vector<int> predecesores;
        
        // Llamar a la función dijkstra para calcular distancias y predecesores
        dijkstra(nodoInicio, n, distancias, predecesores);
        
        char destino;
        cout << "Ingrese el nodo que quieres llegar(Mayusculas): ";
        cin >> destino;

        int destinoIndex = destino - 65;
        if (destinoIndex >= 0 && destinoIndex < n) {
            if (distancias[destinoIndex] == INT_MAX) {
                cout << "El nodo " << destino << " no conecta con A\n";
            } else {
                cout << destino << " " << distancias[destinoIndex] << "\n";

                vector<char> camino;
                // Llamar a la función para reconstruir el camino
                reconstruirCamino(destinoIndex, predecesores, camino);

                cout << "Camino: ";
                for (int i = 0; i < camino.size(); i++) {
                    cout << camino[i];
                    if (i < camino.size() - 1) cout << "->";
                }
                cout << "\n";
            }
        } else {
            cout << "El nodo ingresado no es válido.\n";
        }
    }
}

int main() {
    string archivo;
    cout<<"Ingrese el nombre del archivo: ";
    cin>>archivo;
    LeerArchivo(archivo);
    procesarRuta();
    

    return 0;
}
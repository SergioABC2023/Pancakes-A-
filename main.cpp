#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <set>

using namespace std;

// Estructura que representa un estado del juego de pancakes
struct Estado {
    vector<char> pancakes;
    int costo; // cantidad de volteos realizados
    vector<int> camino; // lista de los índices de los pancakes volteados

    Estado(vector<char> p, int c, vector<int> cam) {
        pancakes = p;
        costo = c;
        camino = cam;
    }
};


// Función que imprime el camino de volteos de pancakes para llegar a la solución
void imprimir_camino(vector<int> camino) {
    for (int i = 0; i < camino.size(); i++) {
        cout << camino[i] << " ";
    }
    cout << endl;
}

// Función que verifica si el estado actual es la solución (pancakes ordenados de forma ascendente)
bool es_solucion(vector<char> pancakes) {
    vector<char> pancakes_ordenados = pancakes;
    sort(pancakes_ordenados.begin(), pancakes_ordenados.end());
    return pancakes == pancakes_ordenados;
}

// Función que realiza el volteo de los pancakes
void voltear_pancakes(vector<char>& pancakes, int desde, int hasta) {
    while (desde < hasta) {
        char temp = pancakes[desde];
        pancakes[desde] = pancakes[hasta];
        pancakes[hasta] = temp;
        desde++;
        hasta--;
    }
}

// Función que imprime el arreglo de pancakes
void imprimir_pancakes(vector<char> pancakes) {
    for (int i = 0; i < pancakes.size(); i++) {
        cout << pancakes[i] << " ";
    }
    cout << endl;
}

// Función heurística que calcula la cantidad de pancakes desordenados
int heuristica(vector<char> pancakes) {
    int desordenados = 0;
    for (int i = 0; i < pancakes.size() - 1; i++) {
        if (pancakes[i] > pancakes[i+1]) {
            desordenados++;
        }
    }
    return desordenados;
}

// Función que define el orden de la cola de prioridad para la búsqueda A*
class OrdenEstado {
public:
    bool operator() (Estado& a, Estado& b) {
        return (a.costo + heuristica(a.pancakes)) > (b.costo + heuristica(b.pancakes));
    }
};

// Función que realiza la búsqueda con A*
void busqueda_con_a(vector<char> pancakes) {

    Estado estado_inicial(pancakes, 0, {});

    // cola de prioridad
    priority_queue<Estado, vector<Estado>, OrdenEstado> cola;
    cola.push(estado_inicial);


    set<vector<char>> visitados;
    visitados.insert(estado_inicial.pancakes);

    while (!cola.empty()) {

        Estado estado_actual = cola.top();
        cola.pop();


        if (es_solucion(estado_actual.pancakes)) {
            cout << "Voltear pancakes: ";
            imprimir_camino(estado_actual.camino);
            cout << " "<< endl;
            cout << "Arreglo ordenado: ";
            imprimir_pancakes(estado_actual.pancakes);
            cout << " "<< endl;
            cout << "Nodos visitados: " << estado_actual.costo << endl;
            return;
            break;
        }

        // Funcion para generar los nuevos estados a partir de los pancakes actuales
        for (int i = 1; i <= estado_actual.pancakes.size(); i++) {
            for (int j = i+1; j <= estado_actual.pancakes.size(); j++) {
                vector<char> nuevos_pancakes = estado_actual.pancakes;
                voltear_pancakes(nuevos_pancakes, i-1, j-1);
                if (visitados.find(nuevos_pancakes) == visitados.end()) {

                    int nuevo_costo = estado_actual.costo + 1;
                    vector<int> nuevo_camino = estado_actual.camino;
                    nuevo_camino.push_back(i-1);
                    nuevo_camino.push_back(j-1);
                    Estado nuevo_estado(nuevos_pancakes, nuevo_costo, nuevo_camino);
                    cola.push(nuevo_estado);
                    visitados.insert(nuevos_pancakes);
                }
            }
        }
    }
}


// Función principal
int main() {
    string entrada;
    vector<char> pancakes;

    cout << "Pancakes con busqueda en A* ";
    cout << " "<< endl;
    cout << "Ingrese los caracteres a ordenar(letras) Ejemplo( flrpmend ) caracteres: ";
    getline(cin, entrada);
    istringstream iss(entrada);

    char c;
    while (iss >> c) {
        pancakes.push_back(c);
    }

    cout << "Arreglo de pancakes: ";
    imprimir_pancakes(pancakes);

    busqueda_con_a(pancakes);
    return 0;
}

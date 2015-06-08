#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <winbgim.h>
#include "../include/LinkedList.h"

using namespace std;

LinkedList<int>* graph = new LinkedList<int>[100]; //Grafo generico.
LinkedList<int>* maze = new LinkedList<int>[100];
LinkedList<int>* mazeAux = new LinkedList<int>[100]; //Grafo para graficar.
vector<int> treasures; //Lista encargada de generar los tesoros aleatorios.
vector<int> currentTreasures; //Lista que contiene los tesoros del nivel actual.
bool* visited = new bool[100];
bool looser;
int currentNode;
int timeToSolve; //Tiempo minimo para resolver el laberinto.
int collectedTreasures;
int help; //Cantidad de veces que se ha usado la ayuda.

bool allTrue(bool* pArray){ //Revisa que en un array de booleanos todos sean true.
    for (int i = 0; i < 100; i++){
        if (pArray[i] == false){
            return false;
        }
    }
    return true;
}

bool isATreasureThere(int pElement){ //Revisa si en la posicion actual hay un tesoro.
    for (int i = 0; i < 5; i++){
        if (currentTreasures[i] == pElement){
            return true;
        }
    }
    return false;
}

void addTreasures(){ //Encargada de anadir los tesoros de la partida.
    random_shuffle(treasures.begin(), treasures.end());
    for (int i = 0; i < 5; i++){
        currentTreasures.push_back(treasures[i]);
    }
}

void dijkstra(int pNodo){ //Algoritmo de ruta mas corta.
    int* distancias = new int[100];
    int* ruta = new int[100];
    bool* definitivos = new bool[100];
    int vecino, menor, nodoActual;
    for (int i = 0; i < 100; i++){
        distancias[i] = 10000; //Inicializa las distancias con valores "infinitos"
        ruta[i] = -1;
        definitivos[i] = false; //Inicializa el arreglo de definitivos en false
    }
    distancias[pNodo] = 0; //Distancia al nodo inicial es 0
    ruta[pNodo] = pNodo; //El nodo proviene de si mismo
    definitivos[pNodo] = true; //El nodo inicial se marca como definitivo
    nodoActual = pNodo;
    while(!allTrue(definitivos)){ //Mientras no se haya llegado al destino
        for (maze[nodoActual].goToStart(); maze[nodoActual].getPos() < maze[nodoActual].getSize(); maze[nodoActual].next()){
            vecino = maze[nodoActual].getElement();
            if (!definitivos[vecino]){ //Si el vecino no ha sido visitado
                if (distancias[nodoActual] + 1 < distancias[vecino]){ //Si la distancia acumulada mas la distancia del vecino (1) es menor a la actual
                    distancias[vecino] = distancias[nodoActual] + 1;
                    ruta[vecino] = nodoActual;
                }
            }
        }
        menor = 10000;
        for (int i = 0; i < 100; i++){ //Busca el menor peso entre los nodos no definitivos
            if (!definitivos[i]){
                if(distancias[i] < menor){ //Si el actual nodo menor es mayor a la posicion en el arreglo de distancias
                    menor = distancias[i];
                    nodoActual = i; //Asigna el nuevo nodo actual
                }
            }
        }
        definitivos[nodoActual] = true; //Marca el nodo actual como definitivo
    }
    nodoActual = 99;
    vecino = ruta[nodoActual];
    setfillstyle(SOLID_FILL, 14);
    while (nodoActual != pNodo){ //Ciclo encargado de poner en pantalla la solucion.
        bar(20 + (vecino % 10) * (40), 20 + (vecino / 10) * (40), 40 + (nodoActual % 10) * (40), 40 + (nodoActual / 10) * (40));
        bar(20 + (nodoActual % 10) * (40), 20 + (nodoActual / 10) * (40), 40 + (vecino % 10) * (40), 40 + (vecino / 10) * (40));
        nodoActual = vecino;
        vecino = ruta[nodoActual];
    }
    delete distancias;
    delete ruta;
    delete definitivos;
}

void depthFirstSearch(int pNode){ //Algoritmo de busqueda en profundidad.
    vector<int> shuffleList;
    int nextNode;
    visited[pNode] = true; //Marca el nodo actual como visitado
    for (graph[pNode].goToStart(); graph[pNode].getPos() < graph[pNode].getSize(); graph[pNode].next()){ //Saca todos los vecinos del nodo actual
        shuffleList.push_back(graph[pNode].getElement());
    }
    random_shuffle(shuffleList.begin(), shuffleList.end()); //Revuelve la lista de vecinos
    for (int i = 0; i < graph[pNode].getSize(); i++){
        nextNode = shuffleList[i];
        if (!visited[nextNode]){ //Si el vecino no ha sido visitado:
            maze[pNode].append(nextNode); //Anade el arco al
            maze[nextNode].append(pNode); //arbol de expansion
            mazeAux[pNode].append(nextNode);
            depthFirstSearch(nextNode);
        }
    }
}

void addRandomPaths(){ //Anade 10 arcos verticales al laberinto/
    vector<int> nodes;
    int randomNode;
    int nodesIndex = 0;
    int donePaths = 0;
    for (int i = 0; i < 90; i++){
        nodes.push_back(i);
    }
    random_shuffle(nodes.begin(), nodes.end()); //Revuelve la lista de arcos.
    while (donePaths < 10){
        randomNode = nodes[nodesIndex];
        if (!maze[randomNode].isThere(randomNode + 10)){ //Si es arco no existe.
            maze[randomNode].append(randomNode + 10);
            maze[randomNode + 10].append(randomNode);
            mazeAux[randomNode].append(randomNode + 10);
            donePaths++;
        }
        nodesIndex++;
    }
}

void defaultGraph(){ //Genera un grafo generico.
    int fila = 0;
    for (int i = 0; i != 100; i++){
        if (i + 10 < 100) {
            graph[i].append(i + 10);
            graph[i + 10].append(i);
        }
        if (i != fila * 10 + 9) {
            graph[i].append(i + 1);
            graph[i + 1].append(i);
        }
        else fila++;
    }
}

void graficador(){ //Pone el laberinto en pantalla.
    int filaNodo, columnaNodo, filaVecino, columnaVecino, vecino;
    int tamanoCurrentTreasures = currentTreasures.size();
    setfillstyle(SOLID_FILL, 15);
    floodfill(0, 0, 7);
    setfillstyle(SOLID_FILL, 0);
    for (int i = 0; i < 100; i++){ //Se mueve al nodo
            filaNodo = i / 10;    //Calcula el numero de fila
            columnaNodo = i % 10; //y columna del nodo
        for (mazeAux[i].goToStart(); mazeAux[i].getPos() < mazeAux[i].getSize(); mazeAux[i].next()){
            vecino = mazeAux[i].getElement(); //Asigna el arco a graficar
            filaVecino = vecino / 10;    //Calcula la fila y
            columnaVecino = vecino % 10; //columna del arco
            if (vecino < i){ //Primero se grafica el mas pequeno
                bar(20 + columnaVecino * (40), 20 + filaVecino * (40), 40 + columnaNodo * (40), 40 + filaNodo * (40));
            }
            else{
                bar(20 + columnaNodo * (40), 20 + filaNodo * (40), 40 + columnaVecino * (40), 40 + filaVecino * (40));
            }
        }
    }
    setfillstyle(8, 14);
    for (int i = 0; i < tamanoCurrentTreasures; i++){ //Grafica los tesoros.
        fillellipse (30 + (currentTreasures[i] % 10) * (40), 30 + (currentTreasures[i] / 10) * (40), 8, 8);
    }
    setfillstyle(3, 14);
    bar (380, 380, 400, 400);
    setfillstyle(SOLID_FILL, 4);
    fillellipse (30 + (currentNode % 10) * (40), 30 + (currentNode / 10) * (40), 8, 8); //Grafica el heroe.
}

void juego(){ //Funcion principal del juego.
    int tecla;
    time_t start = time(0);
    graficador();
    while ((!currentTreasures.empty()) || (currentNode != 99)){
        tecla = getch();
        switch (tecla){
            case 119: //W
            case 72:  //Arriba
                if (maze[currentNode].isThere(currentNode - 10)){
                    currentNode -= 10;
                }
                break;
            case 97: //A
            case 75: //Izquierda
                if (maze[currentNode].isThere(currentNode - 1)){
                    currentNode--;
                }
                break;
            case 100: //D
            case 77:  //Derecha
                if (maze[currentNode].isThere(currentNode + 1)){
                    currentNode++;
                }
                break;
            case 115: //S
            case 80:  //Abajo
                if (maze[currentNode].isThere(currentNode + 10)){
                    currentNode += 10;
                }
                break;
            case 99: //C (Salir)
            case 27: //End (Salir)
                exit(0);
                break;
            case 104: //H (Ayuda)
            case 59:  //F1 (Ayuda)
                if (help < 3){
                    dijkstra(currentNode);
                    help++;
                    continue;
                }
        }
        if (isATreasureThere(currentNode)){
            currentTreasures.erase(std::remove(currentTreasures.begin(), currentTreasures.end(), currentNode), currentTreasures.end()); //Borra el tesoro
            collectedTreasures++;
        }
        graficador();
        if (difftime(time(0), start) > timeToSolve){ //Si se cumplio el tiempo.
            looser = true;
            break;
        }
    }
    if (timeToSolve != 16){ //Resta 3 segundos al tiempo despues de cada nivel.
        timeToSolve -= 3;
    }
}

void init(){ //Reinicia todos los valores.
    cleardevice();
    currentNode = 0;
    timeToSolve = 100;
    currentTreasures.clear();
    for (int i = 0; i < 100; i++){
        maze[i].clear();    //Limpia los
        mazeAux[i].clear(); // grafos
        visited[i] = false; //Marca todos los nodos como no visitados
    }
    depthFirstSearch(0); //Crea el
    addRandomPaths();    //laberinto a
    addTreasures();      //resolver.
}

int main(){
    for(int i = 0; i < 99; i++){ //Inicializa la lista de tesoros.
        treasures.push_back(i);
    }
    help = 0;
    collectedTreasures = 0;
    srand(time(0));
    defaultGraph();
    initwindow(430, 430);
    while (!looser){
        init();
        juego();
    }
    cout << "\nSe acabo el tiempo\n\nTesoros recogidos: " << collectedTreasures << "\n\n"; //Cuando el
    system("pause");                                                                       //jugador pierde.
    return 0;
}

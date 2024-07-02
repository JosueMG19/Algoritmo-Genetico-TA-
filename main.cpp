
/* 
 * File:   main.cpp
 * Author: JOAQUIN
 *
 * Created on 28 de junio de 2024, 04:06 PM
 */

#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#define Tcasamiento 0.5
#define Pcasamiento 0.5 
#define Ttorneo 0.3
#define Tmutacion 0.2
#define IND 5
#define O 3
#define D 4
#define K 7
using namespace std;

struct strnodo{
    int idNodo;
    int demanda;//cantidad de bienes necesaria del nodo
    int inventario;//inventario actual del nodo
};

int obtenerMenorCosto(double costoTransp[][D], int indDestino, vector<strnodo> 
    &nodosCopia){
    int menorCosto=99, indNodo;
    for(int i=0;i<O;i++){
        if(menorCosto>costoTransp[i][indDestino-O]&&nodosCopia[i].inventario){
            menorCosto=costoTransp[i][indDestino-O];
            indNodo=i;
        }
    }
    return indNodo;
}

int obtenerNodoPrio(vector<int> cromo, int prio){
    for(int i=0;i<cromo.size();i++){
        if(cromo[i]==prio)return i;
    }
}

int calculafitness(vector<int> cromo,strnodo nodos[],double costoTransp[][D]){
    int indDestino, indOrigen, demanda, inventario, costo=0;
    vector<strnodo> nodosCopia;
    for(int i=0;i<K;i++){
        nodosCopia.push_back(nodos[i]);
    }
    for(int i=D;i>0;i--){
        indDestino=obtenerNodoPrio(cromo,i)+O;
        while(nodosCopia[indDestino].demanda){
            indOrigen=obtenerMenorCosto(costoTransp, indDestino,nodosCopia);
            if(nodosCopia[indOrigen].inventario>=nodosCopia[indDestino].demanda){
                costo+=costoTransp[indOrigen][indDestino-O]*
                        nodosCopia[indDestino].demanda;
                nodosCopia[indOrigen].inventario-=
                        nodosCopia[indDestino].demanda;
                nodosCopia[indDestino].demanda=0;
            }
            else{
                costo+=costoTransp[indOrigen][indDestino-O]*
                        nodosCopia[indOrigen].inventario;
                nodosCopia[indDestino].demanda-=
                        nodosCopia[indOrigen].inventario;
                nodosCopia[indOrigen].inventario=0;
            }
        }
    }
    return costo;
}

void muestrapoblacion(vector<vector<int>> poblacion,strnodo nodos[],
        double costoTransp[][D]){
    for(int i=0;i<poblacion.size();i++){
        for(int j=0;j<poblacion[i].size();j++){
            cout << poblacion[i][j] << "  ";
        }
        cout <<" fo="<< calculafitness(poblacion[i],nodos, costoTransp)<<endl;
    }
}

void generarCromo(vector<int> &cromo){
    vector<int> prioridades;
    int ind;
    for(int i=1;i<=D;i++){
        prioridades.push_back(i);
        //llena un vector de enteros con las prioridades
    }
    while(prioridades.size()){
        ind=rand()%prioridades.size();
        //decide de forma aleatoria la prioridad del siguiente elemento del
        //vector
        cromo.push_back(prioridades[ind]);
        //inserta la id que corresponde al indice obtenido, de esta forma
        //se consigue una permutacion aleatoria
        prioridades.erase(prioridades.begin()+ind);
    }
}


void generapoblacioninicial(vector<vector<int>> &poblacion){
    int cont=0;
    srand (time(NULL));
    while(cont<IND){
        vector<int> cromo;
        generarCromo(cromo);
        poblacion.push_back(cromo);
        cont++; 
    }
}

int seleccionPorTorneo(vector<vector<int>> poblacion,strnodo nodos[], 
        double costoTransp[][D] ){
    int indMejor=rand()%poblacion.size(), indIndividuo;
    int t=round(poblacion.size()*Ttorneo);
    for(int i=0;i<t;i++){
        indIndividuo=rand()%poblacion.size();
        if(calculafitness(poblacion[indIndividuo],nodos,costoTransp)<
                calculafitness(poblacion[indMejor],nodos,costoTransp)){
            indMejor=indIndividuo;
        }
    }
    return indMejor;
}

//bool clones(vector<int>&cromo1, vector<int> &cromo2){
//    bool bandera=1;
//    for(int i=0;i<cromo1.size();i++){
//        if(cromo1[i]!=cromo2[i]){
//            bandera=0;
//             break;
//        } 
//    }
//    return bandera;
//}

void seleccion(vector<vector<int>> poblacion, vector<vector<int>> &padres,
        strnodo nodos[], double costoTransp[][D]){
    int npadres, indMejor, cont=0;
    bool bandera;
    npadres=round(poblacion.size()*Tcasamiento);
    while(1){
        indMejor=seleccionPorTorneo(poblacion, nodos, costoTransp);
//        bandera=false;
//        for(int i=0;i<padres.size();i++){
//            if(clones(padres[i],poblacion[indMejor])){
//                bandera=true;
//                break;
//            }
//        }
//        if(!bandera){
            padres.push_back(poblacion[indMejor]);
            cont++;
//        }  
        if(cont>=npadres)break;
    }
    
//    muestrapoblacion(padres,paq);
}
void generahijo(vector<int>padre,vector<int>madre,vector<int>&hijo){
    int pos=round(padre.size()*Pcasamiento);
    
    for(int i=0;i<pos;i++)
        hijo.push_back(padre[i]);
    for(int i=pos;i<padre.size();i++)
        hijo.push_back(madre[i]);
    
}


void casamiento(vector<vector<int>> &poblacion,vector<vector<int>> &padres){
    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++)
            if(i!=j){
                vector<int> cromo;
                generahijo(padres[i],padres[j],cromo);
                poblacion.push_back(cromo);
           }
}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> &padres){
    int cont=0, ind1, ind2, aux, indPadre;
    int nmuta=round(poblacion.size()*Tmutacion);
    for(int i=0;i<nmuta;i++){
        vector <int> mutado;
        indPadre=rand()%padres.size();
        ind1=rand()%padres[indPadre].size();
        while(1){
            ind2=rand()%padres[indPadre].size();
                if(ind2!=ind1)break;
        }
        for(int j=0;j<padres[indPadre].size();j++)
            mutado.push_back(padres[indPadre][j]);
        aux=mutado[ind1];
        mutado[ind1]=mutado[ind2];
        mutado[ind2]=aux;
        poblacion.push_back(mutado);
    }    
}

void muestracromo(vector<int>cromo){
    for(int i=0;i<cromo.size();i++){
         cout << cromo[i] << " ";
    }   
     cout << endl;
}

bool aberracion(vector<int> &cromo){
    bool bandera=true;
    for(int i=0;i<cromo.size();i++){
        for(int j=i+1;j<cromo.size();j++){
            if(cromo[i]==cromo[j])
                return true;
        }
    }
    return false;
}

void eliminaberraciones(vector<vector<int>> &poblacion){
    for(int i=0;i<poblacion.size();i++){
        if(aberracion(poblacion[i])){
            poblacion.erase(poblacion.begin()+i); 
            i--;
        }
    }
}

void eliminaclones(vector<vector<int>> &poblacion){
    int bandera;
    for(int i=0;i<poblacion.size();i++){
        for(int j=i+1;j<poblacion.size();j++){
            bandera=1;
            for(int k=0;k<poblacion[i].size();k++){
                if(poblacion[i][k]!=poblacion[j][k]){
                    bandera=0;
                    break;
                } 
            }
            if(bandera){
                poblacion.erase(poblacion.begin()+j);
                j--;
            }
        }
    }
}

int muestramejor(vector<vector<int>> poblacion,
        strnodo nodos[],double costoTransp[][D]){
    int mejor=0;
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[i],nodos,costoTransp)<
                calculafitness(poblacion[mejor],nodos,costoTransp))
            mejor=i;
    
    cout <<"La mejor solucion es:" << calculafitness(poblacion[mejor],
    nodos,costoTransp)<<endl;
    cout<<"Prioridades: "<<endl;
    for(int i=0;i<poblacion[mejor].size();i++){
        cout << poblacion[mejor][i] << "  ";        
    }
    cout << endl;
    return calculafitness(poblacion[mejor],nodos,costoTransp);
}

void imprimelinea(){
    for(int i=0;i<100;i++)
        cout<<"-";
    cout<<endl;
}

void transporteAG(strnodo nodos[],double costoTransp[][D]){
    vector<vector<int>> poblacion;
    int cont=0, mejorAnterior=-1, mejor, genSinMejora=0;
    generapoblacioninicial(poblacion);
    while(1){
        vector<vector<int>> padres;
        cout<<"Poblacion inicial: "<<endl;
        cout<<padres.size()<<endl;
        cout<<poblacion.size()<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        seleccion(poblacion,padres,nodos,costoTransp);
        cout<<"Fase de seleccion:"<<endl;
        cout<<padres.size()<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Padres:"<<endl;
        muestrapoblacion(padres,nodos,costoTransp);
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        casamiento(poblacion,padres);
        cout <<"Casamiento:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        mutacion(poblacion,padres);
        cout <<"Mutacion:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        eliminaberraciones(poblacion);
        eliminaclones(poblacion);
        cout <<endl<<"Luego de control de aberraciones:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        mejor=muestramejor(poblacion,nodos,costoTransp);
        if(mejor==mejorAnterior)
            genSinMejora++;
        else{
            mejorAnterior=mejor;
            genSinMejora=0;
        }   
        cont++;
        if(cont==100||genSinMejora==3) break;    
        
    }    
}

int main(int argc, char** argv) {
    strnodo nodos[K]={{1,0,550},
                      {2,0,300},
                      {3,0,450},
                      {4,300,0},
                      {5,350,0},
                      {6,300,0},
                      {7,350,0}};
//    int idNodo;
//    int demanda;
//    int inventario;
    double costoTransp[O][D]={{11,14,17,13},
                              {16,19,18,15},
                              {15,16,19,18}};
    transporteAG(nodos, costoTransp);
    return 0;
}


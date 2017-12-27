/*
* By Poulkas
*/
#ifndef TABLASIMBOLOS_H_INCLUDED
#define TABLASIMBOLOS_H_INCLUDED
#include <iostream>
#include <vector>
using namespace std;

class NodoTabla{
public:
    string simbolo;
    char tipo;
    NodoTabla(string s, char t){
        simbolo = s;
        tipo = t;
    }
    ~NodoTabla(){}
};

class TablaSimbolos{
public:
    vector<NodoTabla*> tabla;
    TablaSimbolos(){}
    NodoTabla* busca(string s){
        NodoTabla* nodo = NULL;
        for(int i=0, j=(int)tabla.size(); i<j; i++){
            if(tabla.at(i)->simbolo==s){
                nodo = tabla.at(i);
                break;
            }
        }
        return nodo;
    }
    ~TablaSimbolos(){
        while(!tabla.empty()){
            delete tabla.back();
            tabla.pop_back();
        }
    }
    void agregaSimbolo(string s, char t){
        tabla.push_back(new NodoTabla(s, t));
    }
};

#endif // TABLASIMBOLOS_H_INCLUDED

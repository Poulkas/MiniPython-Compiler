/*
* By Poulkas
*/
#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED
#include <iostream>
#include <vector>
#include "ProtosAndDefs.h"

class Pila{
    vector<int> lista;
    int actual, contador;
public:
    Pila(){
        actual = contador = C_0;
    }
    void crearNuevaLista(vector<int> listaTokens){
        for(int i = C_0, j = (int)listaTokens.size(); i<j; i++){
            if(listaTokens.at(i)==TOKEN_LN){
                while(listaTokens.at(i)==TOKEN_LN){
                    lista.push_back(TOKEN_LN);
                    if((i+C_1)<j)
                        i++;
                    else break;
                }
                while(listaTokens.at(i)==TOKEN_TAB){
                    contador++;
                    if((i+C_1)<j)
                        i++;
                    else break;
                }
                if(contador>actual){
                    lista.push_back(TOKEN_A_COR_LLAVE);
                    actual = contador;
                }else{
                    while(contador<actual){
                        cout<<"actual="<< actual <<", "<<"contador="<< contador <<endl;
                        lista.push_back(TOKEN_C_COR_LLAVE);
                        actual--;
                    }
                }
                contador = C_0;
            }
            lista.push_back(listaTokens.at(i));
        }
    }
    vector<int> dameNuevaLista(){
        return lista;
    }
};


#endif // PILA_H_INCLUDED

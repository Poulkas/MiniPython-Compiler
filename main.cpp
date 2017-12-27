/*
* By Poulkas
*/
#include <iostream>
#include <stdlib.h>
//#include "AnalizadorSintactico.h"
//#include "AnalizadorLexico.h"
#include "AnalizadorSemantico.h"

using namespace std;

int main(){
    //AnalizadorLexico analizador;
    //AnalizadorSintactico analizador;
    AnalizadorSemantico analizador;

    analizador.analiza();

    cout<<"Press <ENTER> to continue...";
    cin.get();

    return EXIT_SUCCESS;
}

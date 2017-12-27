/*
* By Poulkas
*/
#include <iostream>
#include <stdlib.h>
//#include "AnalizadorSintactico.h"
//#include "AnalizadorLexico.h"
#include "AnalizadorSemantico.h"

int main(){
    //AnalizadorLexico analizador;
    //AnalizadorSintactico analizador;
    AnalizadorSemantico analizador;

    analizador.analiza();


    cin.get();

    return EXIT_SUCCESS;
}

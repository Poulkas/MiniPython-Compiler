/*
* By Poulkas
*/
#ifndef PROTOSANDDEFS_H_INCLUDED
#define PROTOSANDDEFS_H_INCLUDED
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>

#define WIDTH_A_LEXICO 36
#define HEIGHT_A_LEXICO 27
#define MAX_ASCII 256
#define NOMBRE_ARCHIVO_ENTRADA "entrada.txt"
#define NOMBRE_ARCHIVO_SALIDA "salida.asm"
#define NOMBRE_ARCHIVO_SALIDA_XML "salida.xml"
#define Q25 25
#define Q30 30
#define K 35
#define CHAR_COMILLA char(39)
#define CHAR_TABULADOR char(9)

using namespace std;

enum {C_0, C_1, C_2, C_3, C_4, C_5, C_6, C_7, C_8, C_9, C_10,
      C_11, C_12, C_13, C_14, C_15, C_16, C_17, C_18, C_19, C_20,
      C_21, C_22, C_23, C_24, C_25, C_26, C_27, C_28, C_29, C_30,
      C_31, C_32, C_33, C_34, C_35, C_36, C_37
};
enum {
    TOKEN_RESERVADA,
    TOKEN_ID,
    TOKEN_ENTERO,
    TOKEN_DELIMITADOR,
    TOKEN_A_PARENTESIS,
    TOKEN_C_PARENTESIS,
    TOKEN_A_COR_LLAVE, /* { */
    TOKEN_C_COR_LLAVE, /* } */
    TOKEN_OP_ADICION,
    TOKEN_OP_RESTA,
    TOKEN_OP_MULTI,
    TOKEN_OP_DIV,
    TOKEN_OP_MODULO,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_OP_IGUALDAD,
    TOKEN_OP_MENOR_Q,
    TOKEN_OP_MAYOR_Q,
    TOKEN_OP_MENOR_I_Q,
    TOKEN_OP_MAYOR_I_Q,
    TOKEN_OP_DIF_IGUALDAD,
    TOKEN_OP_AND,
    TOKEN_OP_OR,
    TOKEN_FLOTANTE,
    TOKEN_D_COMILLAS,
    TOKEN_CADENA,
    TOKEN_D_PUNTOS,
    TOKEN_PUNTO,
    TOKEN_A_COR_CUA, /* [ */
    TOKEN_C_COR_CUA, /* ] */
    TOKEN_COMILLA,
    TOKEN_CARACTER,
    TOKEN_ASIGNACION,
    TOKEN_COMA,
    TOKEN_ESPACIO,
    TOKEN_LN,
    TOKEN_TAB,
    TOKEN_FIN
};

int cont = -1;

/* 0.-, 1.-, 2.-, 3.-, 4.-, 5.-, 6.- */
int automataLexico[WIDTH_A_LEXICO][HEIGHT_A_LEXICO]={
    {1,2,35,4,5,6,7,8,9,25,10,30,11,23,22,16,15,17,18,19,13,26,28,27,3,34,35}, /* 0 */
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35}, /* 10 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,32,14,0,14,14,14,35},
    {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,32,14,0,14,14,14,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,0,0,35},
    {0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,0,21,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,0,0,0,0,0,0,35}, /* 20 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,0,20,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,0,26,26,26,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35}, /* 30 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35},
    {35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35} /* 35 */
};

vector<string> P_RESERVADAS = {
    "if",
    "else",
    "while",
    "print"
};

void imprimeToken(int n){
    switch(n){
        case C_0: cout<<"TOKEN_RESERVADA"<<endl;
            break;
        case C_1: cout<<"TOKEN_ID"<<endl;
            break;
        case C_2: cout<<"TOKEN_ENTERO"<<endl;
            break;
        case C_3: cout<<"TOKEN_DELIMITADOR"<<endl;
            break;
        case C_4: cout<<"TOKEN_A_PARENTESIS"<<endl;
            break;
        case C_5: cout<<"TOKEN_C_PARENTESIS"<<endl;
            break;
        case C_6: cout<<"TOKEN_A_COR_LLAVE"<<endl;
            break;
        case C_7: cout<<"TOKEN_C_COR_LLAVE"<<endl;
            break;
        case C_8: cout<<"TOKEN_OP_ADICION"<<endl;
            break;
        case C_9: cout<<"TOKEN_RESTA"<<endl;
            break;
        case C_10:cout<<"TOKEN_OP_MULTI"<<endl;
            break;
        case C_11: cout<<"TOKEN_OP_DIV"<<endl;
            break;
        case C_12: cout<<"TOKEN_OP_MODULO"<<endl;
            break;
        case C_13: cout<<"TOKEN_AND"<<endl;
            break;
        case C_14: cout<<"TOKEN_OR"<<endl;
            break;
        case C_15: cout<<"TOKEN_NOT"<<endl;
            break;
        case C_16: cout<<"TOKEN_OP_IGUALDAD"<<endl;
            break;
        case C_17: cout<<"TOKEN_OP_MENOR_Q"<<endl;
            break;
        case C_18: cout<<"TOKEN_OP_MAYOR_Q"<<endl;
            break;
        case C_19: cout<<"TOKEN_OP_MENOR_I_Q"<<endl;
            break;
        case C_20: cout<<"TOKEN_OP_MAYOR_I_Q"<<endl;
            break;
        case C_21: cout<<"TOKEN_OP_DIF_IGUALDAD"<<endl;
            break;
        case C_22: cout<<"TOKEN_OP_AND"<<endl;
            break;
        case C_23: cout<<"TOKEN_OP_OR"<<endl;
            break;
        case C_24: cout<<"TOKEN_FLOTANTE"<<endl;
            break;
        case C_25: cout<<"TOKEN_D_COMILLAS"<<endl;
            break;
        case C_26: cout<<"TOKEN_CADENA"<<endl;
            break;
        case C_27: cout<<"TOKEN_D_PUNTOS"<<endl;
            break;
        case C_28: cout<<"TOKEN_PUNTO"<<endl;
            break;
        case C_29: cout<<"TOKEN_A_COR_CUA"<<endl;
            break;
        case C_30: cout<<"TOKEN_C_COR_CUA"<<endl;
            break;
        case C_31: cout<<"TOKEN_COMILLA"<<endl;
            break;
        case C_32: cout<<"TOKEN_CARACTER"<<endl;
            break;
        case C_33: cout<<"TOKEN_ASIGNACION"<<endl;
            break;
        case C_34: cout<<"TOKEN_COMA"<<endl;
            break;
        case C_35: cout<<"TOKEN_ESPACIO"<<endl;
            break;
        case C_36: cout<<"TOKEN_LN"<<endl;
            break;
        case C_37: cout<<"TOKEN_TAB"<<endl;
    }
}

bool compruebaTipoVar(string s){
    return s=="int" || s=="float" || s=="void";
}

string operadorXml(string simbolo){
    if(simbolo=="<")
        return "&gt;";
    else if(simbolo==">")
        return "&lt;";
    else if(simbolo=="<=")
        return "&gt;=";
    else if(simbolo==">=")
        return "&lt;=";
    return simbolo;
}

void Error(string s){
    cout<< s <<endl;
}

#endif // PROTOSANDDEFS_H_INCLUDED

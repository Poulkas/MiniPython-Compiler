/*
* By Poulkas
*/
#ifndef ANALIZADORLEXICO_H_INCLUDED
#define ANALIZADORLEXICO_H_INCLUDED
#include "ProtosAndDefs.h"
#include "Pila.h"

class AnalizadorLexico{
    char simbolo;
    string reservada;
    int estado, siguiente, estadoAnterior, position, posToken, posSimbolo, posReservada;
    bool valido;
    vector<int> listaToken;
    vector<string> tablaSimbolo;
    fstream archivo;
    Pila pila;
public:
    fstream archivoSalida;
    AnalizadorLexico();
    ~AnalizadorLexico();
    void analiza();
    int siguienteEstado();
    void escribeToken();
    bool isChar();
    bool isCharNum();
    bool isOpAdicion();
    bool isOpRelacional();
    bool isCharAscii();
    bool isResrved();
    void agregaSimbolo();
    void imprimeListaSimbolos();
    void sigToken();
    void sigSimbolo();
    int damePosToken();
    int dameToken();
    string dameSimbolo();
    bool lexicoValido();
    bool finTokens();
    void imprimeTokens();
};

AnalizadorLexico::AnalizadorLexico(){
    string pReservada;

    archivo.open(NOMBRE_ARCHIVO_ENTRADA);
    archivoSalida.open(NOMBRE_ARCHIVO_SALIDA, ios_base::out | ios_base::trunc);

    estado = siguiente = posToken = posSimbolo = posReservada = C_0;
    reservada = "";
    valido = true;
}

AnalizadorLexico::~AnalizadorLexico(){
    archivo.close();
    archivoSalida.close();
}

void AnalizadorLexico::analiza(){
    archivo.seekg(C_0, ios_base::end);
    archivo.write("\n", C_1);
    archivo.seekg(C_0, ios_base::beg);
    archivo.get(simbolo);
    reservada = reservada+simbolo;
    while(!archivo.eof()){
        siguiente = siguienteEstado();
        estado = automataLexico[estado][siguiente];
        if((estado==C_0 || estado==C_19 || estado==C_32)){
            reservada.erase(reservada.length()-C_1);
            escribeToken();
            if(archivo.eof())
                break;
        }else archivo.get(simbolo);

        reservada = reservada+simbolo;

        if(estado==K){
            valido = false;
            cout<<"Cracter invalido ("<< simbolo <<")"<<endl;
            break;
        }
    }
    pila.crearNuevaLista(listaToken);
    listaToken = pila.dameNuevaLista();
    listaToken.push_back(TOKEN_FIN);
    cout<<"Lexico Valido = "<< valido <<endl;
}

int AnalizadorLexico::siguienteEstado(){
    estadoAnterior = estado;
    if(isChar()){
        return C_0;
    }
    else if(isCharNum()){
        return C_1;
    }
    else if(simbolo=='('){
        return C_3;
    }
    else if(simbolo==')'){
        return C_4;
    }
    else if(simbolo=='{'){
        return C_5;
    }
    else if(simbolo=='}'){
        return C_6;
    }
    else if(isOpAdicion()){
        return C_7;
    }
    else if(simbolo=='*' || simbolo=='%'){
        return C_8;
    }
    else if(simbolo=='/'){
        return C_9;
    }
    else if(simbolo=='&'){
        return C_10;
    }
    else if(simbolo=='|'){
        return C_11;
    }
    else if(isOpRelacional()){
        return C_12;
    }
    else if(simbolo=='!'){
        return C_13;
    }
    else if(simbolo=='='){
        return C_14;
    }
    else if(simbolo=='.'){
        return C_15;
    }
    else if(simbolo==':'){
        return C_16;
    }
    else if(simbolo=='['){
        return C_17;
    }
    else if(simbolo==']'){
        return C_18;
    }
    else if(simbolo==CHAR_COMILLA){
        return C_19;
    }
    else if(simbolo=='"'){
        return C_20;
    }
    else if(simbolo=='#'){
        return C_21;
    }
    else if(simbolo=='\n' || simbolo==' '){
        return C_22;
    }
    else if(simbolo==CHAR_TABULADOR){
        return C_23;
    }
    else if(simbolo==';'){
        return C_24;
    }
    else if(simbolo==','){
        return C_25;
    }
    else if(isCharAscii()){
        return C_2;
    }

    return C_26;
}

void AnalizadorLexico::escribeToken(){
    //cout<<"estadoAnterior="<< estadoAnterior <<endl;
    switch(estadoAnterior){
        case C_1:
            if(isResrved()){
                listaToken.push_back(TOKEN_RESERVADA);
            }else{
                listaToken.push_back(TOKEN_ID);
            }
            agregaSimbolo();
            break;
        case C_2:
            listaToken.push_back(TOKEN_ENTERO);
            agregaSimbolo();
            break;
        case C_3:
            listaToken.push_back(TOKEN_DELIMITADOR);
            break;
        case C_4:
            listaToken.push_back(TOKEN_A_PARENTESIS);
            break;
        case C_5: listaToken.push_back(TOKEN_C_PARENTESIS);
            break;
        case C_6: listaToken.push_back(TOKEN_A_COR_LLAVE);
            break;
        case C_7: listaToken.push_back(TOKEN_C_COR_LLAVE);
            break;
        case C_8:
            if(reservada=="+")
                listaToken.push_back(TOKEN_OP_ADICION);
            else listaToken.push_back(TOKEN_OP_RESTA);
            break;
        case C_9:
        case C_25:
            if(reservada=="*")
                listaToken.push_back(TOKEN_OP_MULTI);
            else if(reservada=="/")
                listaToken.push_back(TOKEN_OP_DIV);
            else
                listaToken.push_back(TOKEN_OP_MODULO);
            break;
        case C_10:
        case C_23:
        case C_30:
            if(reservada=="&")
                listaToken.push_back(TOKEN_AND);
            else if(reservada=="|")
                listaToken.push_back(TOKEN_OR);
            else listaToken.push_back(TOKEN_NOT);
            break;
        case C_11:
        case C_24:
        case C_31:
            if(reservada=="&&")
                listaToken.push_back(TOKEN_OP_AND);
            else if(reservada=="||")
                listaToken.push_back(TOKEN_OP_OR);
            else if(reservada=="==")
                listaToken.push_back(TOKEN_OP_IGUALDAD);
            else if(reservada=="<")
                listaToken.push_back(TOKEN_OP_MENOR_Q);
            else if(reservada==">")
                listaToken.push_back(TOKEN_OP_MAYOR_Q);
            else if(reservada=="<=")
                listaToken.push_back(TOKEN_OP_MENOR_I_Q);
            else if(reservada==">=")
                listaToken.push_back(TOKEN_OP_MAYOR_I_Q);
            else listaToken.push_back(TOKEN_OP_DIF_IGUALDAD);
            break;
        case C_12: listaToken.push_back(TOKEN_FLOTANTE);
            agregaSimbolo();
            break;
        case C_13:
        case C_32: listaToken.push_back(TOKEN_D_COMILLAS);
            break;
        case C_14: listaToken.push_back(TOKEN_CADENA);
            break;
        case C_15: listaToken.push_back(TOKEN_D_PUNTOS);
            break;
        case C_16: listaToken.push_back(TOKEN_PUNTO);
            break;
        case C_17: listaToken.push_back(TOKEN_A_COR_CUA);
            break;
        case C_18: listaToken.push_back(TOKEN_C_COR_CUA);
            break;
        case C_19:
        case C_33: listaToken.push_back(TOKEN_COMILLA);
            break;
        case C_20:
        case C_21: listaToken.push_back(TOKEN_CARACTER);
            break;
        case C_22: listaToken.push_back(TOKEN_ASIGNACION);
            break;
        case C_27: listaToken.push_back(TOKEN_TAB);
            break;
        case C_28:
            if(reservada=="\n")
                listaToken.push_back(TOKEN_LN);
            break;
        case C_34:
            listaToken.push_back(TOKEN_COMA);
    }
    reservada = "";
}

bool AnalizadorLexico::isChar(){
    return isalpha(simbolo) || simbolo=='_';
}

bool AnalizadorLexico::isCharNum(){
    return isalnum(simbolo);
}

bool AnalizadorLexico::isOpAdicion(){
    return simbolo=='-' || simbolo=='+';
}

bool AnalizadorLexico::isOpRelacional(){
    return simbolo=='<' || simbolo=='>';
}

bool AnalizadorLexico::isCharAscii(){
    int ascii = (int)simbolo;
    return ascii>=C_0 && ascii<=MAX_ASCII;
}

bool AnalizadorLexico::isResrved(){
    bool isReserved = false;
    for(int i=C_0, j=(int)P_RESERVADAS.size(); i<j; i++){
        if(reservada==P_RESERVADAS.at(i)){
            isReserved = true;
            break;
        }
    }
    return isReserved;
}

void AnalizadorLexico::agregaSimbolo(){
    tablaSimbolo.push_back(reservada);
}

void AnalizadorLexico::imprimeListaSimbolos(){
    cout<<"Tabla de Simbolos"<<endl;
    for(int i=C_0, j=tablaSimbolo.size(); i<j; i++){
        cout<< i <<" | "<< tablaSimbolo.at(i) <<endl;
    }
}

void AnalizadorLexico::sigToken(){
    if((posToken+C_1)<(int)listaToken.size())
        posToken++;
}

void AnalizadorLexico::sigSimbolo(){
    posSimbolo++;
}

int AnalizadorLexico::damePosToken(){
    return posToken;
}

int AnalizadorLexico::dameToken(){
    return listaToken.at(posToken);
}

string AnalizadorLexico::dameSimbolo(){
    return tablaSimbolo.at(posSimbolo);
}

bool AnalizadorLexico::lexicoValido(){
    return valido;
}

bool AnalizadorLexico::finTokens(){
    return (posToken+C_1)>=(int)listaToken.size();
}

void AnalizadorLexico::imprimeTokens(){
    for(int i=C_0, j=(int)listaToken.size(); i<j; i++){
        switch(listaToken.at(i)){
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
}

#endif // ANALIZADORLEXICO_H_INCLUDED

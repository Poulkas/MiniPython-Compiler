/*
* By Poulkas
*/
#ifndef ANALIZADORSINTACTICO_H_INCLUDED
#define ANALIZADORSINTACTICO_H_INCLUDED
#include "AnalizadorLexico.h"
#include "Arbol.h"

class AnalizadorSintactico{
    bool gramaticaValida;
    Sentencia* programa;
public:
    AnalizadorLexico *lexico;
    AnalizadorSintactico();
    ~AnalizadorSintactico();
    void analiza();
    Sentencia* TRANS_UNIT();
    Sentencia* STATE_P();
    Sentencia* STATE();
    Mientras* ITER_STATE();
    Seleccion* SEL_STATE();
    Asignacion* ASS_EX();
    Print* PRINT_STATE();
    Expresion* EXPRESION();
    Expresion* ADD_EX_P();
    Expresion* ADD_EX();
    Expresion* M_EX_P();
    Expresion* M_EX();
    Expresion* P_EX();
    Expresion* UNARY();
    void SALTO_LINEA();
    void imprimeArbol();
    void imprimeSentencias(Sentencia* sent);
    void imprimeExpresion(Expresion* exp);
    void imprimeExp(Expresion* exp);
    Sentencia* damePrograma();
    bool valido();
    void escribe(bool a);
};

AnalizadorSintactico::AnalizadorSintactico(){
    lexico = new AnalizadorLexico();
    gramaticaValida = true;
    programa = NULL;

    lexico->analiza();
}

AnalizadorSintactico::~AnalizadorSintactico(){
    delete lexico;
    if(programa)
        delete programa;
}

void AnalizadorSintactico::analiza(){
    if(lexico->lexicoValido()){
        //lexico->imprimeTokens();
        //lexico->imprimeListaSimbolos();
        if(lexico->finTokens())
            gramaticaValida = false;
        else programa= TRANS_UNIT();
        cout<<"Gramatica = "<< gramaticaValida <<endl;
        //lexico->archivoSalida << gramaticaValida;
        //imprimeArbol();
        if(gramaticaValida)
            imprimeArbol();
    }

}

Sentencia* AnalizadorSintactico::TRANS_UNIT(){
    Sentencia* aux = NULL;
    cout<<"TRANS_UNIT"<<endl;
    if(!lexico->finTokens() && gramaticaValida){
        aux = STATE();
        if(aux)
            aux->sig = TRANS_UNIT();
    }

    return aux;
}

Sentencia* AnalizadorSintactico::STATE_P(){
    Sentencia* aux = NULL;
    cout<<"STATE_P"<<endl;
    aux = STATE();
    if(lexico->dameToken()==TOKEN_ID || lexico->dameToken()==TOKEN_RESERVADA
       || lexico->dameToken()==TOKEN_LN){
       aux->sig = STATE_P();
    }

    return aux;
}

Sentencia* AnalizadorSintactico::STATE(){
    Sentencia* aux = NULL;
    cout<<"STATE"<<endl;
    SALTO_LINEA();
    if(lexico->dameToken()==TOKEN_ID){
        aux = new Sentencia(ASS_EX());
    }else if(lexico->dameToken()==TOKEN_RESERVADA){
        lexico->sigToken();
        if(lexico->dameSimbolo()=="if"){
            aux = new Sentencia(SEL_STATE());
        }else if(lexico->dameSimbolo()=="while"){
            aux = new Sentencia(ITER_STATE());
        }else if(lexico->dameSimbolo()=="print"){
            aux = new Sentencia(PRINT_STATE());
        }else{
            Error("1.-No se reconoce '"+lexico->dameSimbolo()+"'");
            gramaticaValida = false;
        }
    }

    return aux;
}

Mientras* AnalizadorSintactico::ITER_STATE(){
    Mientras* aux = NULL;
    cout<<"ITER_STATE"<<endl;
    if(lexico->dameSimbolo()=="while"){
        lexico->sigSimbolo();
        aux = new Mientras(EXPRESION());
        if(lexico->dameToken()==TOKEN_D_PUNTOS){
            lexico->sigToken();
            SALTO_LINEA();
            if(lexico->dameToken()==TOKEN_A_COR_LLAVE){
                lexico->sigToken();
                aux->sent = STATE_P();
                if(lexico->dameToken()==TOKEN_C_COR_LLAVE){
                    lexico->sigToken();
                }else{
                    Error("2.-Se esperaba '}'");
                    gramaticaValida = false;
                }
            }else{
                Error("3.-Se esperaba '{'");
                gramaticaValida = false;
            }
        }else{
            Error("4.-Se esperaba ':'");
            gramaticaValida = false;
        }
    }

    return aux;
}

Seleccion* AnalizadorSintactico::SEL_STATE(){
    Seleccion* aux = NULL;
    cout<<"SEL_STATE"<<endl;
    if(lexico->dameSimbolo()=="if"){
        lexico->sigSimbolo();
        aux = new Seleccion(EXPRESION());
        if(lexico->dameToken()==TOKEN_D_PUNTOS){
            lexico->sigToken();
            SALTO_LINEA();
            if(lexico->dameToken()==TOKEN_A_COR_LLAVE){
                lexico->sigToken();
                aux->sent_if = STATE_P();
                if(lexico->dameToken()==TOKEN_C_COR_LLAVE){
                    lexico->sigToken();
                }else{
                    Error("8.-Se esperaba '}'");
                    gramaticaValida = false;
                }
            }else{
                Error("9.-Se esperaba '{'");
                gramaticaValida = false;
            }
        }
    }
    SALTO_LINEA();
    if(lexico->dameToken()==TOKEN_RESERVADA){
        if(lexico->dameSimbolo()=="else"){
            lexico->sigToken();
            lexico->sigSimbolo();
            if(lexico->dameToken()==TOKEN_D_PUNTOS){
                lexico->sigToken();
                SALTO_LINEA();
                if(lexico->dameToken()==TOKEN_A_COR_LLAVE){
                    lexico->sigToken();
                    aux->sent_else = STATE_P();
                    if(lexico->dameToken()==TOKEN_C_COR_LLAVE){
                        lexico->sigToken();
                    }else{
                        gramaticaValida = false;
                    }
                }else{
                    Error("10.-Se esperaba 'TABULADOR'");
                    gramaticaValida = false;
                }
            }
        }
    }

    return aux;
}

Print* AnalizadorSintactico::PRINT_STATE(){
    Print* aux = NULL;
    cout<<"PRINT_STATE"<<endl;
    if(lexico->dameSimbolo()=="print"){
        lexico->sigSimbolo();
        if(lexico->dameToken()==TOKEN_A_PARENTESIS){
            lexico->sigToken();
            aux = new Print(EXPRESION());
            if(lexico->dameToken()==TOKEN_C_PARENTESIS){
                lexico->sigToken();
            }else{
                Error("Se esperaba ')'");
                gramaticaValida = false;
            }
        }else{
            Error("Se esperaba '('");
            gramaticaValida = false;
        }
    }

    return aux;
}

Asignacion* AnalizadorSintactico::ASS_EX(){
    Asignacion* aux = NULL;
    Identificador* id = NULL;
    cout<<"ASS_EX"<<endl;
    if(lexico->dameToken()==TOKEN_ID){
        id = new Identificador(lexico->dameSimbolo());
        lexico->sigToken();
        lexico->sigSimbolo();
        if(lexico->dameToken()==TOKEN_ASIGNACION){
            lexico->sigToken();
            aux = new Asignacion(id, EXPRESION());
            SALTO_LINEA();
        }else if(lexico->dameToken()==TOKEN_A_PARENTESIS){
            lexico->sigToken();
            aux = new Asignacion(id, EXPRESION());
            if(lexico->dameToken()==TOKEN_C_PARENTESIS){
                lexico->sigToken();
                SALTO_LINEA();
            }else{
                gramaticaValida = false;
            }
        }else{
            gramaticaValida = false;
        }
    }else{
        gramaticaValida = false;
    }

    return aux;
}

Expresion* AnalizadorSintactico::EXPRESION(){
    Expresion* aux = NULL;
    cout<<"EXPRESION"<<endl;
    aux = ADD_EX_P();
    if(lexico->dameToken()==TOKEN_OP_MAYOR_Q){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), ">");
            //aux->simbolo = "&gt;";
        }
    }else if(lexico->dameToken()==TOKEN_OP_MENOR_Q){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), "<");
            //aux->simbolo = "&lt;";
        }
    }else if(lexico->dameToken()==TOKEN_OP_MENOR_I_Q){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), "<=");
            //aux->simbolo = "&lt;=";
        }
    }else if(lexico->dameToken()==TOKEN_OP_MAYOR_I_Q){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), ">=");
            //aux->simbolo = "&gt;=";
        }
    }else if(lexico->dameToken()==TOKEN_OP_IGUALDAD){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), "==");
        }
    }else if(lexico->dameToken()==TOKEN_OP_DIF_IGUALDAD){
        lexico->sigToken();
        if(aux){
            aux = new Relacional(aux, EXPRESION(), "!=");
        }
    }

    return aux;
}

Expresion* AnalizadorSintactico::ADD_EX_P(){
    Expresion* aux = NULL;
    cout<<"ADD_EX_P"<<endl;
    aux = new Expresion(M_EX_P(), ADD_EX());
    /*if(aux->der){
        aux->tipo = aux->der->tipo;
        aux->simbolo = aux->der->simbolo;
        aux->der->tipo = ' ';
        aux->der->simbolo = " ";
    }*/

    return aux;
}

Expresion* AnalizadorSintactico::ADD_EX(){
    Expresion* aux = NULL;
    cout<<"ADD_EX"<<endl;
    if(lexico->dameToken()==TOKEN_OP_ADICION){
        lexico->sigToken();
        aux = new Suma(M_EX_P(), ADD_EX(), "+");
    }else if(lexico->dameToken()==TOKEN_OP_RESTA){
        lexico->sigToken();
        aux = new Suma(M_EX_P(), ADD_EX(), "-");
    }

    return aux;
}

Expresion* AnalizadorSintactico::M_EX_P(){
    Expresion* aux = NULL;
    cout<<"M_EX_P"<<endl;
    aux = new Expresion(P_EX(), M_EX());
    /*if(aux->der){
        aux->tipo = aux->der->tipo;
        aux->simbolo = aux->der->simbolo;
        aux->der->tipo = ' ';
        aux->der->simbolo = " ";
    }*/

    return aux;
}

Expresion* AnalizadorSintactico::M_EX(){
    Expresion* aux = NULL;
    cout<<"M_EX"<<endl;
    if(lexico->dameToken()==TOKEN_OP_MULTI){
        lexico->sigToken();
        aux = new Multi(P_EX(), M_EX(), "*");
    }else if(lexico->dameToken()==TOKEN_OP_DIV){
        lexico->sigToken();
        aux = new Multi(P_EX(), M_EX(), "/");
    }

    return aux;
}

Expresion* AnalizadorSintactico::P_EX(){
    Expresion* aux = NULL;
    cout<<"P_EX"<<endl;
    if(lexico->dameToken()==TOKEN_ID){
        aux = new Identificador(lexico->dameSimbolo());
        lexico->sigToken();
        lexico->sigSimbolo();
    }else if(lexico->dameToken()==TOKEN_ENTERO){
        aux = new Entero(lexico->dameSimbolo());
        lexico->sigToken();
        lexico->sigSimbolo();
    }else if(lexico->dameToken()==TOKEN_FLOTANTE){
        aux = new Flotante(lexico->dameSimbolo());
        lexico->sigToken();
        lexico->sigSimbolo();
    }else if(lexico->dameToken()==TOKEN_OP_ADICION || lexico->dameToken()==TOKEN_OP_RESTA){
        aux = UNARY();
    }else if(lexico->dameToken()==TOKEN_A_PARENTESIS){
        lexico->sigToken();
        aux = EXPRESION();
        if(lexico->dameToken()==TOKEN_C_PARENTESIS){
            lexico->sigToken();
        }else{
            Error("11.-Se esperaba ')'");
            gramaticaValida = false;
        }
    }

    return aux;
}

Expresion* AnalizadorSintactico::UNARY(){
    Expresion* aux = NULL;
    if(lexico->dameToken()==TOKEN_OP_ADICION){
        lexico->sigToken();
        aux = new Unary(UNARY(), "+");
    }else if(lexico->dameToken()==TOKEN_OP_RESTA){
        lexico->sigToken();
        aux = new Unary(UNARY(), "-");
    }else aux = P_EX();
    return aux;
}

void AnalizadorSintactico::SALTO_LINEA(){
    cout<<"SALTO_LINEA"<<endl;
    if(lexico->dameToken()==TOKEN_LN){
        lexico->sigToken();
        SALTO_LINEA();
    }
}

void AnalizadorSintactico::imprimeArbol(){
    Sentencia *aux = programa;

    lexico->archivoSalida <<"<PROGRAMA>\n";
    while(aux){
        if(aux->asig){
            lexico->archivoSalida <<"<ASIGNACION>\n";
            lexico->archivoSalida <<"<ID>"+aux->asig->id->simbolo+"</ID>\n";
            imprimeExp(aux->asig->exp);
            lexico->archivoSalida <<"</ASIGNACION>\n";
        }else if(aux->mientras){
            lexico->archivoSalida <<"<MIENTRAS>\n";
            imprimeExpresion(aux->mientras->exp);
            lexico->archivoSalida <<"<BLOQUE>\n";
            imprimeSentencias(dynamic_cast<Sentencia*> (aux->mientras->sent));
            lexico->archivoSalida <<"</BLOQUE>\n";
            lexico->archivoSalida <<"</MIENTRAS>\n";
        }else if(aux->sel){
            lexico->archivoSalida <<"<SI>\n";
            imprimeExpresion(aux->sel->exp);
            lexico->archivoSalida <<"<BLOQUE>\n";
            imprimeSentencias(dynamic_cast<Sentencia*> (aux->sel->sent_if));
            lexico->archivoSalida <<"</BLOQUE>\n";
            if(aux->sel->sent_else){
                lexico->archivoSalida <<"<OTRO>\n";
                imprimeSentencias(dynamic_cast<Sentencia*> (aux->sel->sent_else));
                lexico->archivoSalida <<"</OTRO>\n";
            }lexico->archivoSalida <<"</SI>\n";
        }else if(aux->print){
            lexico->archivoSalida <<"<IMPRIME>\n";
            imprimeExpresion(aux->print->exp);
            lexico->archivoSalida <<"</IMPRIME>\n";
        }
        aux = aux->sig;
    }
    lexico->archivoSalida <<"</PROGRAMA>\n";
}

void AnalizadorSintactico::imprimeSentencias(Sentencia* sent){
    Sentencia* aux = sent;

    while(aux){
        if(aux->asig){
            lexico->archivoSalida <<"<ASIGNACION>\n";
            lexico->archivoSalida <<"<ID>"+aux->asig->id->simbolo+"</ID>\n";
            imprimeExp(aux->asig->exp);
            lexico->archivoSalida <<"</ASIGNACION>\n";
        }else if(aux->mientras){
            lexico->archivoSalida <<"<MIENTRAS>\n";
            imprimeExpresion(aux->mientras->exp);
            lexico->archivoSalida <<"<BLOQUE>\n";
            imprimeSentencias(dynamic_cast<Sentencia*> (aux->mientras->sent));
            lexico->archivoSalida <<"</BLOQUE>\n";
            lexico->archivoSalida <<"</MIENTRAS>\n";
        }else if(aux->sel){
            lexico->archivoSalida <<"<SI>\n";
            imprimeExpresion(aux->sel->exp);
            lexico->archivoSalida <<"<BLOQUE>\n";
            imprimeSentencias(dynamic_cast<Sentencia*> (aux->sel->sent_if));
            lexico->archivoSalida <<"</BLOQUE>\n";
            if(aux->sel->sent_else){
                lexico->archivoSalida <<"<OTRO>\n";
                imprimeSentencias(dynamic_cast<Sentencia*> (aux->sel->sent_else));
                lexico->archivoSalida <<"</OTRO>\n";
            }lexico->archivoSalida <<"</SI>\n";
        }else if(aux->print){
            lexico->archivoSalida <<"<IMPRIME>\n";
            imprimeExpresion(aux->print->exp);
            lexico->archivoSalida <<"</IMPRIME>\n";
        }
        aux = aux->sig;
    }
}

void AnalizadorSintactico::imprimeExpresion(Expresion* exp){
    if(exp->tipo=='v'){
        lexico->archivoSalida <<"<ID>"+exp->simbolo+"</ID>\n";
        //lexico->archivoSalida <<exp->simbolo+" ";
        cout<<"ID"<<endl;
    }else if(exp->tipo=='i'){
        cout<<"ENTERO"<<endl;
        lexico->archivoSalida <<"<ENTERO>"+exp->simbolo+"</ENTERO>\n";
        //lexico->archivoSalida <<exp->simbolo+" ";
    }else if(exp->tipo=='r'){
        cout<<"REAL"<<endl;
        lexico->archivoSalida <<"<REAL>"+exp->simbolo+"</REAL>\n";
        //lexico->archivoSalida <<exp->simbolo+" ";
    }else if(exp->tipo=='s'){
        cout<<"SUMA"<<endl;
        lexico->archivoSalida <<"<SUMA value=\""+exp->simbolo+"\">\n";
        imprimeExp(exp->izq);
        imprimeExp(exp->der);
        lexico->archivoSalida <<"</SUMA>\n";
    }else if(exp->tipo=='m'){
        cout<<"MULTI"<<endl;
        lexico->archivoSalida <<"<MULT value=\""+exp->simbolo+"\">\n";
        //lexico->archivoSalida <<exp->simbolo+" ";
        imprimeExp(exp->izq);
        imprimeExp(exp->der);
        lexico->archivoSalida <<"</MULT>\n";
    }else if(exp->tipo=='o'){
        cout<<"SIGNO"<<endl;
        lexico->archivoSalida <<"<SIGNO value=\""+exp->simbolo+"\">\n";
        imprimeExp(exp->izq);
        lexico->archivoSalida <<"</SIGNO>\n";
    }else if(exp->tipo=='c'){
        lexico->archivoSalida <<"<EXPRESION value=\""+operadorXml(exp->simbolo)+"\">\n";
        imprimeExp(exp->izq);
        imprimeExp(exp->der);
        lexico->archivoSalida <<"</EXPRESION>\n";
    }else{
        lexico->archivoSalida <<"<EXPRESION>\n";
        imprimeExp(exp->izq);
        imprimeExp(exp->der);
        lexico->archivoSalida <<"</EXPRESION>\n";
    }
}

void AnalizadorSintactico::imprimeExp(Expresion* exp){
    if(exp){
        if(exp->tipo=='v'){
            lexico->archivoSalida <<"<ID>"+exp->simbolo+"</ID>\n";
            //lexico->archivoSalida <<exp->simbolo+" ";
            cout<<"ID"<<endl;
        }else if(exp->tipo=='i'){
            cout<<"ENTERO"<<endl;
            lexico->archivoSalida <<"<ENTERO>"+exp->simbolo+"</ENTERO>\n";
            //lexico->archivoSalida <<exp->simbolo+" ";
        }else if(exp->tipo=='r'){
            cout<<"REAL"<<endl;
            lexico->archivoSalida <<"<REAL>"+exp->simbolo+"</REAL>\n";
            //lexico->archivoSalida <<exp->simbolo+" ";
        }else if(exp->tipo=='s'){
            cout<<"SUMA"<<endl;
            lexico->archivoSalida <<"<SUMA value=\""+exp->simbolo+"\">\n";
            imprimeExp(exp->izq);
            imprimeExp(exp->der);
            lexico->archivoSalida <<"</SUMA>\n";
        }else if(exp->tipo=='m'){
            cout<<"MULTI"<<endl;
            lexico->archivoSalida <<"<MULT value=\""+exp->simbolo+"\">\n";
            //lexico->archivoSalida <<exp->simbolo+" ";
            imprimeExp(exp->izq);
            imprimeExp(exp->der);
            lexico->archivoSalida <<"</MULT>\n";
        }else if(exp->tipo=='o'){
            cout<<"SIGNO"<<endl;
            lexico->archivoSalida <<"<SIGNO value=\""+exp->simbolo+"\">\n";
            imprimeExp(exp->izq);
            lexico->archivoSalida <<"</SIGNO>\n";
        }else{
            imprimeExp(exp->izq);
            imprimeExp(exp->der);
        }
    }
}

Sentencia* AnalizadorSintactico::damePrograma(){
    return programa;
}

bool AnalizadorSintactico::valido(){
    return gramaticaValida;
}

void AnalizadorSintactico::escribe(bool a){
    lexico->archivoSalida << a;
}

#endif // ANALIZADORSINTACTICO_H_INCLUDED

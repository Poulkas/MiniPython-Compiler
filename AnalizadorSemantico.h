/*
* By Poulkas
*/
#ifndef ANALIZADORSEMANTICO_H_INCLUDED
#define ANALIZADORSEMANTICO_H_INCLUDED
#include "ProtosAndDefs.h"
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "TablaSimbolos.h"

class AnalizadorSemantico{
public:
    fstream archivoSalida;
    AnalizadorSintactico* sintactico;
    Sentencia* programa;
    TablaSimbolos tabla;
    bool semanticoValido;
    char tipoActual;
    AnalizadorSemantico();
    ~AnalizadorSemantico();
    void analiza();
    void analizaSemantica(Sentencia* sent);
    void analizaExpresion(Expresion* exp);
};

AnalizadorSemantico::AnalizadorSemantico(){
    sintactico = new AnalizadorSintactico();
    semanticoValido = true;
    tipoActual = ' ';
    archivoSalida.open(NOMBRE_ARCHIVO_SALIDA, ios_base::out | ios_base::trunc);
}

AnalizadorSemantico::~AnalizadorSemantico(){
    delete sintactico;
    archivoSalida.close();
}

void AnalizadorSemantico::analiza(){
    string codigo;
    sintactico->analiza();
    sintactico->lexico->archivoSalida.close();
    if(sintactico->valido()){
        programa = sintactico->damePrograma();
        analizaSemantica(programa);
    }
    cout<<"Semantico Valido = "<< semanticoValido <<endl;
    if(semanticoValido){
        codigo = ".386\n"
                 ".model flat, stdcall\n"
                 "option casemap:none; labels are case-sensitive now\n"
                 "include \\masm32\\macros\\macros.asm\n"
                 "include \\masm32\\include\\masm32.inc\n"
                 "include \\masm32\\include\\kernel32.inc\n"
                 "includelib \\masm32\\lib\\masm32.lib\n"
                 "includelib \\masm32\\lib\\kernel32.lib\n"
                 ".data\n"
                 ".data?\n";
        for(auto &s: tabla.tabla)
            codigo += s->simbolo+" dword ?\n";
        codigo += ".code\n"
                  "inicio:\n";
        codigo += programa->generaCodigo();
        codigo += "exit\n"
                  "end inicio";
        archivoSalida << codigo;
    }
}

void AnalizadorSemantico::analizaSemantica(Sentencia* sent){
    Sentencia* aux = sent;
    NodoTabla*  temp = NULL;

    while(aux){
        if(aux->asig){
            analizaExpresion(aux->asig->exp);
            temp = tabla.busca(aux->asig->id->simbolo);
            if(!temp){
                aux->asig->id->tipo = tipoActual;
                cout<<"Tipo="<< tipoActual <<endl;
                tabla.agregaSimbolo(aux->asig->id->simbolo, tipoActual);
            }else{
                if(temp->tipo!=tipoActual)
                    tipoActual = 'e';
            }
        }else if(aux->mientras){
            analizaExpresion(aux->mientras->exp);
            analizaSemantica(dynamic_cast<Sentencia*> (aux->mientras->sent));
        }else if(aux->sel){
            analizaExpresion(aux->sel->exp);
            analizaSemantica(dynamic_cast<Sentencia*> (aux->sel->sent_if));
            if(aux->sel->sent_else)
                analizaSemantica(dynamic_cast<Sentencia*> (aux->sel->sent_else));
        }else if(aux->print){
            analizaExpresion(aux->print->exp);
        }
        aux = aux->sig;
        if(tipoActual=='e'){
            semanticoValido = false;
            break;
        }else{
            tipoActual = ' ';
        }
    }
}

void AnalizadorSemantico::analizaExpresion(Expresion* exp){
    NodoTabla* aux;
    if(exp){
        if(exp->tipo=='v'){
            aux = tabla.busca(exp->simbolo);
            if(aux){
                if(tipoActual==' ' || tipoActual==aux->tipo){
                    tipoActual = aux->tipo;
                }else tipoActual = 'e';
                cout<<"Tipo="<< tipoActual <<endl;
            }else tipoActual = 'e';
        }else if(exp->tipo=='i'){
            cout<<"ENTERO"<<endl;
            if(tipoActual==' ' || tipoActual=='i'){
                tipoActual='i';
            }else if(tipoActual=='r'){
                tipoActual='e';
            }
        }else if(exp->tipo=='r'){
            cout<<"REAL"<<endl;
            if(tipoActual==' ' || tipoActual=='r'){
                tipoActual='r';
            }else{
                tipoActual='e';
            }
        }else if(exp->tipo=='s'){
            exp->tipo = tipoActual;
            analizaExpresion(exp->izq);
            analizaExpresion(exp->der);
        }else if(exp->tipo=='m'){
            exp->tipo = tipoActual;
            analizaExpresion(exp->izq);
            analizaExpresion(exp->der);
        }else if(exp->tipo=='o'){
            exp->tipo = tipoActual;
            cout<<"SIGNO"<<endl;
            analizaExpresion(exp->izq);
        }else{
            analizaExpresion(exp->izq);
            analizaExpresion(exp->der);
        }
    }
}

#endif // ANALIZADORSEMANTICO_H_INCLUDED

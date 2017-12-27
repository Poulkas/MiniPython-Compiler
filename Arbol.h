/*
* By Poulkas
*/
#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED
#include <sstream>

class Sentencia;
string generaCodigoSentencia(Sentencia* sent);

class Nodo{
public:
    stringstream numeroCont;
    string simbolo, etiquetaInicio, etiquetaFin;
    char tipo;
    Nodo(){
        simbolo = " ";
        tipo = ' ';
    }
    virtual string generaCodigo()=0;
};

class Expresion: public Nodo{
public:
    Expresion *izq, *der;
    Expresion(){
        izq = NULL;
        der = NULL;
    }
    Expresion(Expresion* i, Expresion* d){
        izq = i;
        der = d;
    }
    string generaCodigo(){
        string codigo = "";
        if(izq)
            codigo += izq->generaCodigo();
        if(der)
            codigo += der->generaCodigo();
        return codigo;
    }
};

class Unary: public Expresion{
public:
    Unary(Expresion* e, string s){
        izq = e;
        simbolo = s;
        tipo = 'o';
    }
    string generaCodigo(){
        string codigo = "";
        if(simbolo=="-"){
            codigo += izq->generaCodigo();
            codigo += "pop eax\n";
            codigo += "neg eax\n";
            codigo += "push eax\n";
        }
        return codigo;
    }
};

class Relacional: public Expresion{
public:
    Relacional(Expresion* i, Expresion* d, string s){
        izq = i;
        der = d;
        simbolo = s;
        tipo = 'c';
        cont++;
        numeroCont << cont;
    }
    string generaCodigo(){
        string codigo = "", salto = "", etiquetaFin = "Fin", etiquetaTrue = "Verdadero";
        if(simbolo==">")
            salto = "jg";
        else if(simbolo=="<")
            salto = "jl";
        else if(simbolo==">=")
            salto = "jge";
        else if(simbolo=="<=")
            salto = "jle";
        else if(simbolo=="==")
            salto = "je";
        else if(simbolo=="!=")
            salto = "jne";

        if(izq)
            codigo += izq->generaCodigo();
        if(der)
            codigo += der->generaCodigo();
        codigo += "pop ebx\n";
        codigo += "pop eax\n";
        codigo += "cmp eax, ebx\n";
        codigo += salto+" "+etiquetaTrue+numeroCont.str()+"\n";
        codigo += "push 0\n";
        codigo += "jmp "+etiquetaFin+numeroCont.str()+"\n";
        codigo += etiquetaTrue+numeroCont.str()+":\n";
        codigo += "push 1\n";
        codigo += etiquetaFin+numeroCont.str()+":\n";
        return codigo;
    }
};

class Identificador: public Expresion{
public:
    Identificador(string s){
        simbolo = s;
        tipo = 'v';
    }
    string generaCodigo(){
        return "push "+ simbolo +"\n";
    }
};

class Entero: public Expresion{
public:
    Entero(string entero){
        simbolo = entero;
        tipo = 'i';
    }
    string generaCodigo(){
        return "push "+ simbolo +"\n";
    }
};

class Flotante: public Expresion{
public:
    Flotante(string flotante){
        simbolo = flotante;
        tipo = 'r';
    }
    string generaCodigo(){
        return "push "+ simbolo +"\n";
    }
};

class Suma: public Expresion{
public:
    Suma(Expresion *i, Expresion *d, string op){
        simbolo = op;
        izq = i;
        der = d;
        tipo = 's';
    }
    string generaCodigo(){
        string codigo = "";
        cout<<"CLASS SUMA"<<endl;
        if(izq)
            codigo += izq->generaCodigo();
        if(der)
            codigo += der->generaCodigo();
        if(simbolo=="+"){
            codigo += "pop ebx\n"
                      "pop eax\n"
                      "add eax, ebx\n"
                      "push eax\n";
        }else if(simbolo=="-"){
            codigo += "pop ebx\n"
                      "pop eax\n"
                      "sub eax, ebx\n"
                      "push eax\n";
        }
        return codigo;
    }
};

class Multi: public Expresion{
public:
    Multi(Expresion *i, Expresion *d, string op){
        simbolo = op;
        izq = i;
        der = d;
        tipo = 'm';
    }
    string generaCodigo(){
        string codigo = "";
        cout<<"CLASS MULTI"<<endl;
        if(izq)
            codigo += izq->generaCodigo();
        if(der)
            codigo += der->generaCodigo();
        if(simbolo=="*"){
            codigo += "pop ebx\n"
                      "pop eax\n"
                      "imul ebx\n"
                      "push eax\n";
        }else if(simbolo=="/"){
            codigo += "pop ebx\n"
                      "pop eax\n"
                      "idiv ebx\n"
                      "push eax\n";
        }
        return codigo;
    }
};

class Asignacion: public Nodo{
public:
    Identificador *id;
    Expresion *exp;
    Asignacion(Identificador *i, Expresion *e){
        id = i;
        exp = e;
    }
    string generaCodigo(){
        string codigo = "";
        codigo += exp->generaCodigo();
        codigo += "pop eax\n";
        codigo += "mov "+ id->simbolo +", eax\n";
        return codigo;
    }
};

class Mientras: public Nodo{
public:
    Expresion* exp;
    Sentencia* sent;
    Mientras(Expresion* e){
        exp = e;
        sent = NULL;
        cont++;
        numeroCont << cont;
    }
    string generaCodigo(){
        string codigo = "", etiquetaWhile = "While"+numeroCont.str(), etiquetaFin = "Fin"+numeroCont.str();
        codigo += etiquetaWhile+":\n";
        codigo += exp->generaCodigo();
        codigo += "pop eax\n";
        codigo += "cmp eax, 1\n";
        codigo += "jne "+etiquetaFin+"\n";
        codigo += generaCodigoSentencia(sent);
        codigo += "jmp "+ etiquetaWhile +"\n";
        codigo += etiquetaFin+":\n";
        return codigo;
    }
};

class Seleccion: public Nodo{
public:
    Expresion* exp;
    Sentencia *sent_if, *sent_else;
    Seleccion(Expresion* e){
        exp = e;
        sent_if = NULL;
        sent_else = NULL;
        cont++;
        numeroCont << cont;
    }
    string generaCodigo(){
        string codigo = "", etiquetaIf = "If"+numeroCont.str(), etiquetaElse = "Else"+numeroCont.str();
        codigo += exp->generaCodigo();
        codigo += "pop eax\n";
        codigo += "cmp eax, 0\n";
        codigo += "je "+ etiquetaElse +"\n";
        codigo += generaCodigoSentencia(sent_if);
        codigo += "jmp "+ etiquetaIf +"\n";
        codigo += etiquetaElse + ":\n";
        if(sent_else)
            codigo += generaCodigoSentencia(sent_else);
        codigo += etiquetaIf+":\n";
        return codigo;
    }
};

class Print: public Nodo{
public:
    Expresion* exp;
    Print(Expresion* e){
        exp = e;
    }
    string generaCodigo(){
        string codigo = "";
        codigo += exp->generaCodigo();
        codigo += "pop eax\n";
        codigo += "print str$(eax)\n";
        codigo += "print chr$(10)\n";
        return codigo;
    }
};

class Sentencia: public Nodo{
public:
    Mientras* mientras;
    Asignacion* asig;
    Seleccion* sel;
    Sentencia* sig;
    Print* print;
    Sentencia(Mientras* m){
        mientras = m;
        asig = NULL;
        sel = NULL;
        sig = NULL;
        print = NULL;
    }
    Sentencia(Asignacion* a){
        mientras = NULL;
        asig = a;
        sel = NULL;
        sig = NULL;
        print = NULL;
    }
    Sentencia(Seleccion* s){
        mientras = NULL;
        asig = NULL;
        sel = s;
        sig = NULL;
        print = NULL;
    }
    Sentencia(Print* p){
        mientras = NULL;
        asig = NULL;
        sel = NULL;
        sig = NULL;
        print = p;
    }
    string generaCodigo(){
        string codigo = "";
        Sentencia* aux = this;
        int i = 0;
        while(aux){
            if(aux->mientras){
                codigo += aux->mientras->generaCodigo()+"\n\n";
            }else if(aux->asig){
                codigo += aux->asig->generaCodigo()+"\n\n";
            }else if(aux->sel){
                codigo += aux->sel->generaCodigo()+"\n\n";
            }else if(aux->print){
                codigo += aux->print->generaCodigo()+"\n\n";
            }
            cout<<"i="<< i <<endl;
            i++;
            aux = aux->sig;
        }
        return codigo;
    }
};

string generaCodigoSentencia(Sentencia* sent){
        return sent->generaCodigo();
    }

#endif // ARBOL_H_INCLUDED

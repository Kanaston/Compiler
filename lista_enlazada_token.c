#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void CrearBuffer(char *buffer){
    char buffer[200], pt; //arreglo de gran tamaño    char *pt;
     int longTexto;

   gets(buffer);   //almacena el nombre en el arreglo

   longTexto =  strlen(buffer);
    if (longTexto==0) return NULL;

   pt = (char*)malloc(sizeof(char) * (longTexto + 1));
         // Se genera el espacio exacto
         // +1 para que almacene el 0 al final de texto

   strcpy(pt,buffer);
   // las dos instrucciones anteriores se pueden reemplazar por:
   // pt = strdup(buffer);

   return pt;
   
  /*  buffer=NULL;
    if(buffer != NULL){
        return 1;
    }else{
        return 0;
    }
    esto lo puse en caso de que llegara con informacion el buffer 
    */
   

}

void asignarPalRes(char *palabra)
{
    if (palabra == "INCIO" || palabra == "FIN" || palabra == "POR" || palabra == "SI" || palabra == "MOSTRAR" || palabra == "REGRESA" || palabra == "ENTONCES" || palabra == "ROMPER" || palabra == "CONTRA" || palabra == "VAR")
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Identificador(char caracter)
{
    if (isalpha(caracter))
    {
        if((isalpha(caracter) || isalnum(caracter))){
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

void Constante(char *palabra)
{
    if(isdigit(palabra))
        return 1;
    else
        return 0;
    
}

void Simbolo(char *palabra)
{
     if (palabra == ";" || palabra == "{" || palabra == "}" || palabra == "<" || palabra == ">" || palabra == ">=" || palabra == "<=" || palabra == "!=" || palabra == "&&" || palabra == "||" || palabra == "!")
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
enum TipoToken
{
    PalRes,
    Id,
    Num,
    Sim
};

struct Token
{
    char Nombre[80];
    enum TipoToken Tipo;
    char Lexema[80];
    int Valor;
    int NoLin;
    int NoCol;
};

struct nodo
{
    struct Token info;
    struct nodo *izq;
    struct nodo *der;
};

struct nodo *raiz = NULL;
struct nodo *actual = NULL;

void insertar(struct Token token)
{
    struct nodo *nuevo;
    nuevo = malloc(sizeof(struct nodo));

    nuevo->info = token;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    if (raiz == NULL)
    {
        raiz = nuevo;
        actual = nuevo;
    }
    else
    {
        nuevo->izq = actual;
        actual->der = nuevo;
        actual = nuevo;
    }
}

void imprimirLista(struct nodo *reco)
{
    if (reco != NULL)
    {
        printf("Nombre: %s\n", reco->info.Nombre);
        printf("Tipo: %d\n", reco->info.Tipo);
        printf("Lexema: %s\n", reco->info.Lexema);
        printf("Valor: %d\n", reco->info.Valor);
        printf("-------\n");
        imprimirLista(reco->der);
    }
}

int main()
{
    struct Token token;
    strcpy(token.Nombre, "ParentesisDer");
    token.Tipo = Sim;
    strcpy(token.Lexema, "(");
    token.Valor = 0;
    //CrearBuffer(token); esto lo puse para mandar a la funcion del buffer

    insertar(token);

    printf("=====LISTA DE TOKENS=====\n");
    imprimirLista(raiz);

    getch();
    return 0;
}

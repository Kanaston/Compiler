#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TipoToken
{
    PalRes = 1,
    Id = 2,
    Num = 3,
    Sim = 4
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

/* Global */
struct nodo *raiz = NULL;
struct nodo *actual = NULL;
char bufferLectura[100] = "";
int estadoAsignacion = 0;
int statusLectura = 0;
char tipoAsignacion = 0;
int columna = 0;
int fila = 1;

int asignarPalRes(char *palabra)
{
    if (strcmp(palabra, "INICIO") == 0 || strcmp(palabra, "FIN") == 0 || strcmp(palabra, "POR") == 0 || strcmp(palabra, "SI") == 0 || strcmp(palabra, "MOSTRAR") == 0 || strcmp(palabra, "REGRESA") == 0 || strcmp(palabra, "ENTONCES") == 0 || strcmp(palabra, "ROMPER") == 0 || strcmp(palabra, "NO") == 0 || strcmp(palabra, "VAR") == 0)
    {
        tipoAsignacion = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

int asignarIdentificador(char caracter)
{
    if (isalpha(caracter) && estadoAsignacion == 0)
    {
        tipoAsignacion = 2;
        estadoAsignacion = 1;
        statusLectura = 1;
        bufferLectura[strlen(bufferLectura)] = caracter;
        return 0;
    }
    else if (isalnum(caracter) && estadoAsignacion == 1)
    {
        bufferLectura[strlen(bufferLectura)] = caracter;
        statusLectura = 1;
        return 0;
    }
    else
    {
        return 1;
    }
}

int asignarConstante(char caracter)
{
    if (isdigit(caracter))
    {
        estadoAsignacion = 1;
        tipoAsignacion = 3;
        statusLectura = 1;
        bufferLectura[strlen(bufferLectura)] = caracter;
        return 0;
    }
    else if (isdigit(caracter) && estadoAsignacion == 1)
    {
        statusLectura = 1;
        return 0;
    }
    else
    {
        return 1;
    }
}

int asignarSimbolo(char caracter)
{
    if (caracter == ';' || caracter == '{' || caracter == '=' || caracter == '}' || caracter == '<' || caracter == '>' || caracter == '&' || caracter == '|' || caracter == '(' || caracter == ')' || caracter == '/' || caracter == '+' || caracter == '-' || caracter == '.' || caracter == '"' || caracter == '!')
    {
        bufferLectura[strlen(bufferLectura)] = caracter;
        statusLectura = 1;
        tipoAsignacion = 4;
        return 1;
    }
    else
    {
        return 1;
    }
}

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
        printf("Numero de Linea: %d\n", reco->info.NoLin);
        // printf("Numero de Columna: %d\n", reco->info.NoCol);
        printf("-------\n");
        imprimirLista(reco->der);
    }
}

void leer(char *path)
{
    FILE *archivo;
    char caracter;
    archivo = fopen(path, "r");
    if (archivo == NULL)
    {
        printf("Fallo en la lectura del archivo %s", path);
    }
    else
    {
        while ((caracter = fgetc(archivo)) != EOF)
        {
            columna++;
            leerToken(caracter);    
            if (caracter == '\n')
            {
                fila++;
                columna = 0;
            }
        }
        leerToken(caracter);
    }
}

void leerToken(char *caracter)
{
    int estadoLectura = 0;
    if (tipoAsignacion == 0 || tipoAsignacion == 2)
    {
        estadoLectura = asignarIdentificador(caracter);
        if (asignarTipoToken(estadoLectura) == 1)
        {
            estadoLectura = 0;
        };
    }
    if (tipoAsignacion == 0 || tipoAsignacion == 3)
    {
        estadoLectura = asignarConstante(caracter);
        if (asignarTipoToken(estadoLectura) == 1)
        {
            estadoLectura = 0;
        };
    }
    if (tipoAsignacion == 0 || tipoAsignacion == 4)
    {
        estadoLectura = asignarSimbolo(caracter);
        if (asignarTipoToken(estadoLectura) == 1)
        {
            estadoLectura = 0;
        };
    }
}

int asignarTipoToken(int estadoLectura)
{
    if (estadoLectura == 1 && statusLectura == 1)
    {
        if (tipoAsignacion == 2)
        {
            asignarPalRes(bufferLectura);
        }
        struct Token token;
        strcpy(token.Nombre, "");
        token.Tipo = tipoAsignacion;
        if (atoi(bufferLectura))
        {
            token.Valor = atoi(bufferLectura);
        }
        else
        {
            token.Valor = 0;
        }
        strcpy(token.Lexema, bufferLectura);
        token.NoCol = columna;
        token.NoLin = fila;
        insertar(token);
        tipoAsignacion = 0;
        estadoAsignacion = 0;
        statusLectura = 0;
        memset(bufferLectura, 0, 100);
        return 1;
    }
    return 0;
}

int main()
{
    printf("=====LISTA DE TOKENS=====\n");
    leer("test.ck");
    imprimirLista(raiz);
    getch();
    return 0;
}

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

enum TipoToken
{
    PalRes,
    Id,
    Num,
    Sim,
    Cad
};

char *TiposVAR[] =
{
    "numero",
    "caracter",
    "cadena",
    "decimal",
    "booleano"
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

struct nodo *raiz;
struct nodo *actual;

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

struct Token GetToken()
{
    if (actual->der == NULL)
    {
        struct Token token;
        strcpy(token.Nombre, "");
        token.Tipo = PalRes;
        strcpy(token.Lexema, "");
        token.Valor = 0;
        return token;
    }
    else
    {
        actual = actual->der;
        return actual->info;
    }
}

int compararVAR(char *lexema)
{
    for (int i = 0; i <  sizeof(TiposVAR)/sizeof(TiposVAR[0]); i++)
    {
        if((strcmp(lexema, TiposVAR[i])==0)){
            return 0;
        }
    }
    return 1;
}

/*-------------------------- VAR --------------------------- */
void sintaxisVAR(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "VAR") == 0)
        {
            edo = 2;
            token = GetToken();
            sintaxisVAR(token, edo);
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
            token = GetToken();
            sintaxisVAR(token, edo);
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ":") == 0)
        {
            edo = 4;
            token = GetToken();
            sintaxisVAR(token, edo);
        }
        break;
    case 4:
        if (compararVAR(token.Lexema))
        {
            edo = 5;
            token = GetToken();
            sintaxisVAR(token, edo);
        }
        break;
    case 5:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 6;
            token = GetToken();
        }
        else if (strcmp(token.Lexema, ",") == 0)
        {
            edo = 2;
            token = GetToken();
            sintaxisVAR(token, edo);
        }
        break;
    default:
        break;
    }
}

/*----------------------- MOSTRAR -------------------------- */
void sintaxisMostrar(struct Token token, int edo)
{
    if (edo == 1)
    {
        if (strcmp(token.Lexema, "MOSTRAR") == 0)
        {
            edo = 2;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        else
        {
            printf("Se esperaba la palabra Mostrar\n");
        }
    }
    if (edo == 2)
    {
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 3;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        else
        {
            printf("Se esperaba un parentesis\n");
        }
    }
    if (edo == 3)
    {
        if (token.Tipo == Cad || token.Tipo == Id)
        {
            edo = 4;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        else
        {
            printf("Se esperaba una cadena\n");
        }
    }
    if (edo == 4)
    {
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 5;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        else
        {
            printf("Se esperaba un parentesis\n");
        }
    }
    if (edo == 5)
    {
        if (token.Tipo == Sim)
        {
            printf("Cadena correcta");
            token = GetToken();
        }
        else
        {
            printf("Se esperaba un ;\n");
        }
    }
}
/*-------------------------------------------------------- */

int main()
{
    // raiz = NULL;
    // actual = NULL;

    // struct Token token;
    // strcpy(token.Nombre, "PALRES");
    // token.Tipo = PalRes;
    // strcpy(token.Lexema, "Mostrar");
    // token.Valor = 0;
    // insertar(token);

    // strcpy(token.Nombre, "ParIzq");
    // token.Tipo = Sim;
    // strcpy(token.Lexema, "(");
    // token.Valor = 0;
    // insertar(token);

    // strcpy(token.Nombre, "CAD");
    // token.Tipo = Cad;
    // strcpy(token.Lexema, "Hola Mundo");
    // token.Valor = 0;
    // insertar(token);

    // strcpy(token.Nombre, "ParDer");
    // token.Tipo = Sim;
    // strcpy(token.Lexema, ")");
    // token.Valor = 0;
    // insertar(token);

    // strcpy(token.Nombre, "PuntoComa");
    // token.Tipo = Sim;
    // strcpy(token.Lexema, ";");
    // token.Valor = 0;
    // insertar(token);
    // printf("=====LISTA DE TOKENS=====\n");
    // actual = raiz;
    // sintaxisMostrar(actual->info, 1);
    printf("%i",sizeof(TiposVAR[0])); 
    getch();
    return 0;
}
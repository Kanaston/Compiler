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
        "booleano"};

char *TiposOPArit[] =
    {
        "*",
        "/",
        "+",
        "-"};

char TiposOPLogSimb[] = {
    "|",
    "&",
    "=",
    "<",
    ">",
    "!"};
char *TiposOpLog[] = {
    "||",
    "&&",
    "!=",
    ">=",
    "<=",
    "=="};

char *TiposBoolean[] = {
    "true",
    "false"};

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
    for (int i = 0; i < sizeof(TiposVAR) / sizeof(TiposVAR[0]); i++)
    {
        if ((strcmp(lexema, TiposVAR[i]) == 0))
        {
            return 0;
        }
    }
    return 1;
}
int compararOPArit(char *lexema)
{
    for (int i = 0; i < sizeof(TiposOPArit) / sizeof(TiposOPArit[0]); i++)
    {
        if ((strcmp(lexema, TiposOPArit[i]) == 0))
        {
            return 0;
        }
    }
    return 1;
}
int compararOPLog(char *lexema)
{
    for (int i = 0; i < sizeof(TiposOPArit) / sizeof(TiposOPArit[0]); i++)
    {
        if ((strcmp(lexema, TiposOPArit[i]) == 0))
        {
            return 0;
        }
    }
    return 1;
}
int compararBoolean(char *lexema)
{
    for (int i = 0; i < sizeof(TiposBoolean) / sizeof(TiposBoolean[0]); i++)
    {
        if ((strcmp(lexema, TiposBoolean[i]) == 0))
        {
            return 0;
        }
    }
    return 1;
}
int compararOpLogSimb(char *lexema)
{
    for (int i = 0; i < sizeof(TiposOPLogSimb) / sizeof(TiposOPLogSimb[0]); i++)
    {
        if ((strcmp(lexema, TiposOPLogSimb[i]) == 0))
        {
            return 0;
        }
    }
    return 1;
}
void compararOpLog(char *lexema)
{
    char *temp = GetToken().Lexema;
    if ((compararOpLogSimb(lexema) == 0) && (compararOpLogSimb(temp) == 0))
    {
        for (int i = 0; i < sizeof(TiposOpLog) / sizeof(TiposOpLog[0]); i++)
        {
            if ((strcmp(lexema, TiposOpLog[i]) == 0))
            {
                return 0;
            }
        }
    }
    return 1;
}
/*--------------------- IF -----------------------*/
void factorElse(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if ((strcmp(token.Lexema, "ELSE") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
            token = GetToken();
            factorElse(token, edo);
        }
        break;
    case 2:
        edo = 3;
        sentencia(token);
        factorElse(token, edo);
        break;
    case 3:
        if ((strcmp(token.Lexema, "END") == 0) && (token.Tipo == PalRes))
        {
            edo = 4;
            token = GetToken();
            factorElse(token, edo);
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "ELSE") == 0) && (token.Tipo == PalRes))
        {
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
void factorElseIf(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if ((strcmp(token.Lexema, "ELSE") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
            token = GetToken();
        }
        break;
    case 2:
        if ((strcmp(token.Lexema, "IF") == 0) && (token.Tipo == PalRes))
        {
            edo = 3;
            token = GetToken();
        }
        break;
    case 3:
        edo = 4;
        expFactor(token, 1);
        token = GetToken();
        break;
    case 4:
        if ((strcmp(token.Lexema, "ELSE") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
            token = GetToken();
        }
        break;
    case 5:
        if ((strcmp(token.Lexema, "IF") == 0) && (token.Tipo == PalRes))
        {
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
void factorIf(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 2;
            token = GetToken();
            factorIf(token, edo);
        }
        break;
    case 2:
        edo = 3;
        expLog(token, 1);
        factorIf(token, edo);
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
            token = GetToken();
            factorIf(token, edo);
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "ENTONCES") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
            token = GetToken();
            factorIf(token, edo);
        }
        break;
    case 5:
        edo = 6;
        sentencia(token);
        factorIf(token, edo);
        break;
    case 6:
        if ((strcmp(token.Lexema, "END") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
            token = GetToken();
            factorIf(token, edo);
        }
        break;
    default:
        break;
    }
}

void sentenciaIF(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if ((strcmp(token.Lexema, "END") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
            token = GetToken();
            sentenciaIf(token, edo);
        }
        break;
    case 2:
        edo = 3;
        factorIf(token, edo);
    case 3:
        if ((strcmp(token.Lexema, "IF") == 0) && (token.Tipo == PalRes))
        {
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
/*-------------- EXPRESION LOGICA ----------------*/
void expLog(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "!") == 0)
        {
            edo = 2;
            token = GetToken();
            expLog(token, edo);
        }
        break;
    case 2:
        if (compararOPLog(token.Lexema) == 0)
        {
            edo = 1;
            token = GetToken();
        }
        else
        {
            expFactor(token, 1);
        }
        break;
    default:
        break;
    }
}
void factorOpLog(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 2;
            token = GetToken();
            factorOpLog(token, edo);
        }
        else if (compararOPLog(token.Lexema) == 0)
        {
            token = GetToken();
            opLog(token, 1);
        }
        else
        {
            if (compararBoolean(token.Lexema) == 0)
            {
                token = GetToken();
            }
        }
        break;
    case 2:
        edo = 3;
        token = GetToken();
        opLog(token, 1);
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
void opLog(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 2;
            token = GetToken();
            opLog(token, edo);
        }
        break;
    case 2:
        if (compararBoolean(token.Lexema)==0)
        {
            edo = 2;
            token = GetToken();
            opLog(token, edo);
        }
        break;
    case 3:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 4;
            token = GetToken();
            opLog(token, edo);
        }
        else
        {
            token = GetToken();
        }
    case 4:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 1;
            token = GetToken();
            opLog(token, edo);
        }
        break;
        break;
    default:
        break;
    }
}
/*------------------- LEER------------------------*/
void leer(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 2;
            token = GetToken();
            leer(token, edo);
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
            token = GetToken();
            leer(token, edo);
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
            token = GetToken();
            leer(token, edo);
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 4;
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
/*-------------------OPERACIONES ARITMETICAS------------------------*/
void expAritOP(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (token.Tipo == Num)
        {
            edo = 2;
            token = GetToken();
            expAritOP(token, edo);
        }
        break;
    case 2:
        if (compararOPArit(token.Lexema))
        {
            edo = 3;
            token = GetToken();
            expAritOP(token, edo);
        }
        else
        {
            token = GetToken();
        }
        break;
    case 3:
        if (token.Tipo == Num)
        {
            edo = 4;
            token = GetToken();
            expAritOP(token, edo);
        }
        break;
    case 4:
        if (compararOPArit(token.Lexema))
        {
            edo = 1;
            token = GetToken();
            expAritOP(token, edo);
        }
        else
        {
            token = GetToken();
        }
        break;
    default:
        break;
    }
}
void expAritmetica(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        expFactor(token, 1);
        edo = 2;
        token = GetToken();
        break;
    case 2:
        if (compararOPArit(token.Lexema) == 0)
        {
            token = GetToken();
            edo = 1;
        }
        break;

    default:
        break;
    }
}
void expFactor(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 2;
            token = GetToken();
        }
        else if (compararOPArit(token.Lexema))
        {
            GetToken();
        }
        break;
    case 2:
        if (compararOPArit(token.Lexema))
        {
            edo = 3;
            token = GetToken();
        }
        break;
    case 3:
        if (strcmp(token.Lexema, "(") == 0)
        {
            token = GetToken();
        }
        else
        {
            GetToken();
        }
        break;
    default:
        break;
    }
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
    }
}

/*----------------------- MOSTRAR -------------------------- */
void sintaxisMostrar(struct Token token, int edo)
{
    switch (edo)
    {
        if (strcmp(token.Lexema, "MOSTRAR") == 0)
        {
            edo = 2;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
    case 2:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 3;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        break;
    case 3:
        if (token.Tipo == Cad || token.Tipo == Id)
        {
            edo = 4;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 5;
            token = GetToken();
            sintaxisMostrar(token, edo);
        }
        break;
    case 5:
        if (token.Tipo == Sim)
        {
            token = GetToken();
        }
        break;
    }
}
/*---------------------------------------------------------*/
void sentencia(struct Token token)
{
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
    printf("%i", sizeof(TiposVAR[0]));
    getch();
    return 0;
}
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
int compararOpLog(char *lexema)
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
        if ((strcmp(token.Lexema, "NO") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
        }
        break;
    case 2:
        edo = 3;
        identificarSentencia(GetToken());
        break;
    case 3:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 4;
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "NO") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
        }
        break;
    default:
        break;
    }
    if (edo != 5)
    {
        factorElse(GetToken(), edo);
    }
}
void factorElseIf(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if ((strcmp(token.Lexema, "NO") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
        }
        break;
    case 2:
        if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
        {
            edo = 3;
        }
        break;
    case 3:
        edo = 4;
        expFactor(GetToken(), 1);
        break;
    case 4:
        if ((strcmp(token.Lexema, "NO") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
        }
        break;
    case 5:
        if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
        {
            edo = 6;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 6)
    {
        factorElseIf(GetToken(), edo);
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
        }
        break;
    case 2:
        edo = 3;
        expLog(GetToken(), 1);
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "ENTONCES") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
        }
        break;
    case 5:
        edo = 6;
        identificarSentencia(GetToken());
        break;
    case 6:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 7;
        }
        break;
    default:
        break;
    }
    if (edo != 7)
    {
        factorIf(GetToken(), edo);
    }
}

void sentenciaSi(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
        }
        break;
    case 2:
        edo = 3;
        factorIf(GetToken(), 1);
    case 3:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 4;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 4)
    {
        sentenciaSi(GetToken(), edo);
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
        }
        else
        {
            edo = 3;
            factorOpLog(GetToken(), 1);
        }
        break;
    case 2:
        edo = 3;
        factorOpLog(GetToken(), 1);
        break;
    case 3:
        if (compararOpLog(token.Lexema) == 0)
        {
            edo = 1;
        }
        else
        {
            edo = 4;
        }
        break;
    default:
        break;
    }
    if (edo != 4)
    {
        expLog(GetToken(), edo);
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
        }
        else
        {
            if (compararBoolean(token.Lexema) == 0)
            {
                edo = 4;
                GetToken();
            }
            else
            {
                edo = 4;
                opLog(GetToken(), 1);
            }
        }
        break;
    case 2:
        edo = 3;
        opLog(GetToken(), 1);
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 4)
    {
        factorOpLog(GetToken(), edo);
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
        }
        break;
    case 2:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 2;
        }
        break;
    case 3:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 4;
        }
        else
        {
            edo = 5;
            GetToken();
        }
        break;
    case 4:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 1;
        }
        break;
    default:
        break;
    }
    if (edo != 5)
    {
        opLog(GetToken(), edo);
    }
}
/*------------------- LEER------------------------*/
void sentenciaLeer(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 2;
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 5;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 5)
    {
        sentenciaLeer(GetToken(), edo);
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
        }
        break;
    case 2:
        if (compararOPArit(token.Lexema))
        {
            edo = 3;
        }
        else
        {
            edo = 5;
            GetToken();
        }
        break;
    case 3:
        if (token.Tipo == Num)
        {
            edo = 4;
        }
        break;
    case 4:
        if (compararOPArit(token.Lexema))
        {
            edo = 1;
        }
        else
        {
            edo = 5;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 5)
    {
        expAritOP(GetToken(), edo);
    }
}
void expAritmetica(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        edo = 2;
        expFactor(token, 1);
        break;
    case 2:
        if (compararOPArit(token.Lexema) == 0)
        {
            edo = 1;
        }
        else
        {
            edo = 3;
            GetToken();
        }
        break;

    default:
        break;
    }
    if (edo != 3)
    {
        expAritmetica(GetToken(), edo);
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
        }
        else if (compararOPArit(token.Lexema))
        {
            edo = 4;
            GetToken();
        }
        break;
    case 2:
        if (compararOPArit(token.Lexema))
        {
            edo = 3;
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
            GetToken();
        }
        else
        {
            edo = 4;
            GetToken();
        }
        break;
    default:
        break;
    }
    if (edo != 4)
    {
        expFactor(GetToken(), edo);
    }
}
/*-------------------------- VAR --------------------------- */
void sentenciaVariable(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "VAR") == 0)
        {
            edo = 2;
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ":") == 0)
        {
            edo = 4;
        }
        break;
    case 4:
        if (compararVAR(token.Lexema))
        {
            edo = 5;
        }
        break;
    case 5:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 6;
            GetToken();
        }
        else if (strcmp(token.Lexema, ",") == 0)
        {
            edo = 2;
        }
        break;
    }
    if (edo != 6)
    {
        sentenciaVariable(GetToken(), edo);
    }
}

/*----------------------- MOSTRAR -------------------------- */
void sentenciaMostrar(struct Token token, int edo)
{
    switch (edo)
    {
        if (strcmp(token.Lexema, "MOSTRAR") == 0)
        {
            edo = 2;
        }
    case 2:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 3;
        }
        break;
    case 3:
        if (token.Tipo == Cad || token.Tipo == Id)
        {
            edo = 4;
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 5;
        }
        break;
    case 5:
        if (token.Tipo == Sim)
        {
            edo = 6;
            token = GetToken();
        }
        break;
    }
    if (edo != 6)
    {
        sentenciaMostrar(GetToken(), edo);
    }
}
/*---------------------------------------------------------*/
void mainProgram(struct Token token, int edo)
{
    switch (edo)
    {
    case 1:
        if (strcmp(token.Lexema, "INICIO") == 0)
        {
            edo = 2;
            mainProgam(GetToken());
        }
        break;

    case 2:
        edo = 3;
        identificarSentencia(GetToken());
        mainProgram(GetToken(), 1);
        break;
    case 3:
        if (strcmp(token.Lexema, "FIN") == 0)
        {
            token = GetToken();
        }
        break;

    default:
        break;
    }
}
void identificarSentencia(struct Token token)
{
    if ((strcmp(token.Lexema, "VAR") == 0) && (token.Tipo == PalRes))
    {
        sentenciaVariable(token,1);
    }
    else if ((strcmp(token.Lexema, "MOSTRAR") == 0) && (token.Tipo == PalRes))
    {
        sentenciaMostrar(token,1);
    }
     else if ((strcmp(token.Lexema, "LEER") == 0) && (token.Tipo == PalRes))
    {
        sentenciaLeer(token,1);
    }
     else if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
    {
        sentenciaSi(token,1);
    }
}    
/*-------------------------------------------------------- */

int main()
{
    getch();
    return 0;
}
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// enum TipoToken
// {
//     PalRes,
//     Id,
//     Num,
//     Sim,
//     Cad
// };

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

char *TiposOPLogSimb[] = {
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


enum TipoToken
{
    PalRes = 1,
    Id = 2,
    Num = 3,
    Sim = 4,
    Cad = 5
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
        else{
            edo=123;
            mostrarError("NO", token);
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
        else{
            edo=123;
            mostrarError("FIN", token);
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "NO") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
        }
        else{
            edo=123;
            mostrarError("NO", token);
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
        else{
            edo=123;
            mostrarError("NO", token);
        }
        break;
    case 2:
        if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
        {
            edo = 3;
        }
        else{
            edo=123;
            mostrarError("SI", token);
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
        else{
            edo=123;
            mostrarError("NO", token);
        }
        break;
    case 5:
        if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
        {
            edo = 6;
            GetToken();
        }
        else{
            edo=123;
            mostrarError("SI", token);
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
        else{
            edo=123;
            mostrarError("(", token);
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
        else{
            edo=123;
            mostrarError(")", token);
        }
        break;
    case 4:
        if ((strcmp(token.Lexema, "ENTONCES") == 0) && (token.Tipo == PalRes))
        {
            edo = 5;
        }
        else{
            edo=123;
            mostrarError("ENTONCES", token);
        }
        break;
    case 5:  
        mainProgram(GetToken(),6);
        break;
    case 6:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 7;
        }
        else{
            edo=123;
            mostrarError("FIN", token);
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
        if ((strcmp(token.Lexema, "SI") == 0) && (token.Tipo == PalRes))
        {
            edo = 2;
        }
        else{
            edo=123;
            mostrarError("SI", token);
        }
        break;
    case 2:
        edo = 3;
        factorIf(GetToken(), 1);
    case 3:
        if ((strcmp(token.Lexema, "FIN") == 0) && (token.Tipo == PalRes))
        {
            edo = 4;
        }
        else{
            edo=123;
            mostrarError("FIN", token);
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
        }
        else{
            edo=123;
            mostrarError(")", token);
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
        else{
            edo=312;
            mostrarError("Un booleano", token);
        }
        break;
    case 2:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 2;
        }
        else{
            edo = 3060;
            mostrarError("Un booleano",token);
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
        }
        break;
    case 4:
        if (compararBoolean(token.Lexema) == 0)
        {
            edo = 1;
        }
        else{
            edo = 30;
            mostrarError("Un booleano", token);
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
        else{
            edo=123;
            mostrarError("(", token);
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
        }
        else{
            edo=423;
            mostrarError("Identificar",token);
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
        }
        else{
            edo=123;
            mostrarError(")", token);
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 5;
        }
        else{
            edo = 30;
            mostrarError(";", token);
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
        else
        {
            edo=12312;
            mostrarError("Un numero",token);
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
        }
        break;
    case 3:
        if (token.Tipo == Num)
        {
            edo = 4;
        }
        else
        {
            edo=12312;
            mostrarError("Un numero",token);
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
        }
        else{
            edo = 30;
            mostrarError("(", token);
        }
        break;
    case 2:
        if (compararOPArit(token.Lexema))
        {
            edo = 3;
        }else{
            edo = 40;
            mostrarError("Un operador aritmetico", token);
        }
        break;
    case 3:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 4;
        }
        else
        {
            edo = 40;
            mostrarError(")", token);
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
        else{
            edo=7;
            mostrarError("VAR", token);
        }
        break;
    case 2:
        if (token.Tipo == Id)
        {
            edo = 3;
        }
        else{
            edo = 30;
            mostrarError("Identificador", token);

        }
        break;
    case 3:
        if (strcmp(token.Lexema, ":") == 0)
        {
            edo = 4;
        }
        else{
            edo=7;
            mostrarError(":", token);
        }
        break;
    case 4:
        if (compararVAR(token.Lexema))
        {
            edo = 5;
        }
        else{
            edo=7;
            mostrarError("Un tipo de dato valido", token);
        }
        break;
    case 5:
        if (strcmp(token.Lexema, ";") == 0)
        {
            edo = 6;
        }
        else if (strcmp(token.Lexema, ",") == 0)
        {
            edo = 2;
        }
        else{
            edo=7;
            mostrarError("; o ,", token);
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
        else
        {
            edo=7;
            mostrarError("MOSTRAR", token);
        }
    case 2:
        if (strcmp(token.Lexema, "(") == 0)
        {
            edo = 3;
        }
        else{
            edo = 7;
            mostrarError("(", token);
        }
        break;
    case 3:
        if (token.Tipo == Cad || token.Tipo == Id)
        {
            edo = 4;
        }
        else{
            edo = 7;
            mostrarError("Cadena o identificador", token);
        }
        break;
    case 4:
        if (strcmp(token.Lexema, ")") == 0)
        {
            edo = 5;
        }
        else{
            edo = 7;
            mostrarError(")", token);
        }
        break;
    case 5:
        if (token.Tipo == Sim && (strcmp(token.Lexema, ";") == 0))
        {
            edo = 6;
            token = GetToken();
        }else
        {
            edo = 7;
            mostrarError(";", token);
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
            mainProgam(GetToken(),2);
        }
        else
        {
            edo = 4;
            mostrarError("INICIO",token);
        }
        break;

    case 2:
        edo = identificarSentencia(GetToken());
        mainProgram(GetToken(), edo);
        break;
    case 3:
        if (strcmp(token.Lexema, "FIN") == 0)
        {
            token = GetToken();
        }
        else
        {
            edo = 4;
            mostrarError("FIN", token);
        }
        break;

    default:
        break;
    }
}

int mostrarError(char* esperado,struct Token token){
    printf("Error en \"%s\" Se esperaba \"%s\" pero se encontro %s",token.NoLin ,esperado, token.Lexema);
}

int identificarSentencia(struct Token token)
{
    int edo = 2;
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
    else{
        edo = 3;
    }
    return edo;
}    
/*-------------------------------------------------------- */

int main()
{
    printf("=====LISTA DE TOKENS=====\n");
    leer("test.ck");
    imprimirLista(raiz);
   mainProgram(actual->info,1);
    getch();
    return 0;
}
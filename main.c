#include <stdio.h>
#include <stdlib.h>

int main()
{

}

char *leer(nombre, espacio)
{
    FILE *archivo;
    char caracter;
    char *ret = malloc(espacio);
    if(!ret)
        return NULL;
    
    archivo = fopen(nombre,"r");
    if (archivo == NULL)
    {
        return NULL;
    }
    else
    {
        caracter = fgetc(archivo);
        while(caracter != EOF)
        {
            printf("%c\n",caracter);
            caracter = fgetc(archivo);
        }
    }
    fclose(archivo);
}

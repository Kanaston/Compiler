#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *archivo;
    char caracter;
    archivo = fopen("Prueba.txt","r");
    if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
    }
    else
    {
        printf("\nEl contenido del archivo de prueba es \n\n");
        caracter = fgetc(archivo);
        while(caracter != EOF)
        {
            printf("%c\n",caracter);
            caracter = fgetc(archivo);
        }
    }
    fclose(archivo);
    return 0;
}

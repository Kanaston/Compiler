#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void leer(char *buffer, char *main_code,int space)
{
    char lector;
    FILE *archivo = fopen(main_code, "r");
    if (archivo == NULL)
    {
        printf("Error: Could not open file\n");
    }
    else
    {
        fread(buffer, sizeof(char), space, archivo);
    }
    fclose(archivo);
}

void leerToken(char caracter){
    switch(caracter){
        
    }
}

int main()
{
    char buffer[256];
    char main_code[] = "test.ck";
    leer(buffer, main_code,256);
    printf("%s", buffer);
    return 0;
}

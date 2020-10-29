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
/*
void PToken(char *caracter)
{
    int i=0;
    int y=0;
    char *prueba=malloc(strlen(caracter)+1);
    while(caracter[i] != '\0'){
        if(caracter[i] == ' ' || caracter[i] == ';' || caracter[i] == '\0'){
            printf("%s\n", prueba);
            //printf("%s\n",prueba);
            memset(prueba, '\0', strlen(prueba));
            y=0;
        }
        else{
            prueba[y]=caracter[i];
            y++;
        }
        i++;
    }
}
*/
int main()
{
    char buffer[256];
    char main_code[] = "test.ck";
    leer(buffer, main_code,256);
    printf("%s", buffer);
    /*PToken(buffer);*/
    return 0;
}

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

#define numPalRes 22
#define numSimEspecial 20

struct nodo {
    char nombre[10];
    char tipo[10];
    char lexema[10];

    struct nodo *siguiente;
    struct nodo *anterior;
};
//Variables
struct nodo *raiz = NULL;
char Token[] = "";

char palRes[numPalRes][10] = {"inicio", "fin", "var", "const", "si", "entonces", "iniciosi", "finsi",
    "sino", "iniciosino", "finsino", "para", "iniciopara", "finpara", "escribir", "leer",
    "entero", "real", "booleano", "cadena" };

char simEspecial[numSimEspecial][3] = {"+", "-", "*", "/", "<", ">", "=", "&", "|", ";", ":", "(", ")",
    ">=", "<=", "!=", ":="};

//Funciones
void identidacadorPalRes(char data[]);
void insertar(char  nombre[], char tipo[], char lexema[]);
void imprimirPre(struct nodo *reco);
void borrar(struct nodo *reco);

int main()
{
    //insertar("nombre_1", "tipo_1", "lexema_1");
    FILE *archivo;
    char caracter;

    archivo = fopen("programa.ng","r");

    if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
    }
    else
    {
        printf("\nEl contenido del archivo de prueba es \n\n");
        while((caracter = fgetc(archivo)) != EOF)
        {
            int control = 0; //si encunetra un simEsp no entra al if de  PalRes
            char aux[] = "";    //Da menos problemas que trabajar con caracter directamente
            strncat(aux, &caracter, 1);

            //Dentro de este if trabajaremos la identificacion
            //Primero descartamos los simbolos que no nos sirven
            if(strcmp(aux, " ") != 0 && strcmp(aux, "\n") != 0 && strcmp(aux, "\t") != 0){
                for(int i = 0; i < numSimEspecial; i++){
                    if(strcmp(aux, simEspecial[i]) == 0){
                    //Al encontrar un ocurrencia insertamos el nodo y ademas
                    //rompemos el ciclo
                    //Falta implementrar los simbolos complejos sera similar al proyecto prueba
                        insertar(aux, "simEsp", aux);
                        control = 1;
                        break;
                    }
                }

               if (control == 0){
                    strncat(Token, aux, 1);//Concatenamos
                    identidacadorPalRes(Token);
                }
            }
        }
    }

    fclose(archivo);

    printf("Nombre\tTipo\tLexema\n\n");
    imprimirPre(raiz);

    borrar(raiz);
    getch();
    return 0;
}

void identidacadorPalRes(char data[]){
    for(int i = 0; i < numPalRes; i++){
        if(strcmp(data, palRes[i]) == 0){
            insertar(data, "PalRes", data);
            strcpy(Token,  ""); //limpiamos la variable
            break;
        }
    }
}

void insertar(char  nombre[], char tipo[], char lexema[]){
    struct nodo *nuevo = NULL, *nAux = raiz;
    nuevo = malloc(sizeof(struct nodo));

    strcpy(nuevo->nombre, nombre);
    strcpy(nuevo->tipo, tipo);
    strcpy(nuevo->lexema, lexema);
    nuevo->anterior = NULL;
    nuevo->siguiente = NULL;

    //printf("%i, x);
    if(raiz == NULL)
        raiz=nuevo;
    else{
        while(nAux->siguiente != NULL)
            nAux = nAux->siguiente;

        nuevo->anterior = nAux;
        nAux->siguiente = nuevo;
    }
}

void imprimirPre(struct nodo *reco){
    while (reco != NULL)
    {
        printf("%s\t",reco->nombre);
        printf("%s\t",reco->tipo);
        printf("%s\n\n",reco->lexema);
        reco = reco->siguiente;
    }
}

void borrar(struct nodo *reco){
    if (reco != NULL)
    {
        borrar(reco->anterior);
        borrar(reco->siguiente);
        free(reco);
    }
}

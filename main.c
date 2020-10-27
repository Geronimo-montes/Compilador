#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <ctype.h>
#include <Windows.h>

#define numPalRes 22
#define numSimEspecial 20

struct nodo
{
    char nombre[30];
    char tipo[30];
    char lexema[30];

    struct nodo *siguiente;
    struct nodo *anterior;
};
//Variables
struct nodo *raiz = NULL;
char Token[] = "\0";
char caracterAnterior;

char palRes[numPalRes][10] = {"inicio", "fin", "var", "const", "si", "entonces", "iniciosi", "finsi",
                              "sino", "iniciosino", "finsino", "para", "iniciopara", "finpara", "escribir", "leer",
                              "entero", "real", "booleano", "cadena"
                             };

char simEspecial[numSimEspecial][3] = { "+", "-", "*", "/", "<", ">", "=", "&", "|", ";", ":", "(", ")",
                                       ">=", "<=", "!=", ":=" };

//Funciones
bool identidacadorPalRes(char data[]);
bool identidacadorIdentificador(char data[]);
void insertar(char  nombre[], char tipo[], char lexema[]);
void imprimirPre(struct nodo *reco);
void borrar(struct nodo *reco);

void Color(int Background, int Text){ // Función para cambiar el color del fondo y/o pantalla
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos la consola.
	// Para cambiar el color, se utilizan números desde el 0 hasta el 255.
	// Pero, para convertir los colores a un valor adecuado, se realiza el siguiente cálculo.
	int New_Color= Text + (Background * 16);
	SetConsoleTextAttribute(Console, New_Color); // Guardamos los cambios en la Consola.
}

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
        while((caracter = fgetc(archivo)) != EOF)
        {
            char simbolo = caracter;
            char aux[] = "";    //Da menos problemas que trabajar con caracter directamente
            bool simboloRaro = true; //0: simbolo sospechoso; 1: es un simbolo especial
            strncat(aux, &caracter, 1);

            //Primero identificamos los simbolos especiales
            switch(simbolo)
            {
            //Casos especificos para los simbolos complejos
            case ':':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    if(strcmp(Token, "\0") != 0)
                        if(!identidacadorPalRes(Token))
                            identidacadorIdentificador(Token);

                    simboloRaro = false;
                }
                break;

            case '!':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    if(strcmp(Token, "\0") != 0)
                        if(!identidacadorPalRes(Token))
                            identidacadorIdentificador(Token);

                    simboloRaro = false;
                }
                break;

            case '>':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    if(strcmp(Token, "\0") != 0)
                        if(!identidacadorPalRes(Token))
                            identidacadorIdentificador(Token);

                    simboloRaro = false;
                }
                break;

            case '<':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    if(strcmp(Token, "\0") != 0)
                        if(!identidacadorPalRes(Token))
                            identidacadorIdentificador(Token);

                    simboloRaro = false;
                }
                break;
            case '=':
                if(strcmp(Token, "\0") != 0)
                    if(!identidacadorPalRes(Token))
                        identidacadorIdentificador(Token);

                if (caracterAnterior == ':')
                {
                    insertar(":=", "SimEsp", ":=");
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '!')
                {
                    insertar("!=", "SimEsp", "!=");
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '>')
                {
                    insertar(">=", "SimEsp", ">=");
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '<')
                {
                    insertar("<=", "SimEsp", "<=");
                    caracterAnterior = '\0';
                }
                else
                {
                    insertar("=", "SimEsp", "=");
                }

                simboloRaro = false;
                break;
            //Saltos de linea, tabulaciones y espacions son idicadores de insertar el
            //contenido de Token
            case '\n':
            case '\t':
            case ' ':
                if(strcmp(Token, "\0") != 0)
                    if(!identidacadorPalRes(Token))
                        identidacadorIdentificador(Token);
                break;
            //En caso de no corresponder a nunguna opcion podemos asumir que se trata
            //de un simEsp simple
            default:
                if(caracterAnterior != '\0')
                {
                    //Puede que el caracter almacenado nunca se use por lo que hay que verificar
                    if(caracterAnterior != '!')
                    {
                        //el signo de exclamacion no es utilizado individualmente por lo que se descarta
                        insertar(&caracterAnterior, "SimEsp", &caracterAnterior);
                    }
                    caracterAnterior = '\0';
                }

                for(int i = 0; i < numSimEspecial; i++)
                {
                    if(strcmp(aux, simEspecial[i]) == 0)
                    {
                        //Al encontrar un ocurrencia insertamos el nodo y rompemos el ciclo
                        if(strcmp(Token, "\0") != 0)
                            if(!identidacadorPalRes(Token))
                                identidacadorIdentificador(Token);
                        //if(!identidacadorIdentificador(Token));

                        //numeros(Token);
                        insertar(aux, "simEsp", aux);
                        simboloRaro = false;
                        break;
                    }
                }

                break;
            }

            if(simbolo != ' ' && simbolo != '\n' && simbolo != '\t')
            {
                if(isalpha(simbolo) || isdigit(simbolo) || simbolo == '_' || simbolo == '.' || simboloRaro)
                {
                    strncat(Token, aux,1);//Concatenamos

                }
            }
        }
    }

    fclose(archivo);

    printf("  Nombre\t\tTipo\t\tLexema\n\n");
    imprimirPre(raiz);

    borrar(raiz);
    getch();
    return 0;
}

bool identidacadorPalRes(char data[])
{
    for(int i = 0; i < numPalRes; i++)
    {
        if(strcmp(data, palRes[i]) == 0)
        {
            insertar(data, "PalRes", data);
            strcpy(Token,  ""); //limpiamos la variable
            return true;
        }
    }
    return false;
}

void numeros(char data[])
{

    if(atoi(data) || atof(data))
    {
        insertar(data, "NUM", data);
        strcpy(Token,  ""); //limpiamos la variable
    }
}

bool identidacadorIdentificador(char data[])
{
    bool valido = true;
    int i = 0;

    if(isalpha(data[0]) || data[0] == '_' )
    {
        while(data[i])
        {
            if(isalnum(data[i]) || data[i] == '_')
            {
                valido = true;
            }
            else
            {
                valido = false;
                insertar(data, "Error", "Cadena invalida");
                strcpy(Token,  ""); //limpiamos la variable
                break;
            }
            i++;
        }
    }
    else if (isdigit(data[0]))
    {
        if(isdigit(data[1]) || data[1] == '.')
        {
            numeros(data);
            valido = false;
        }
        else if(isalpha(data[1]))
        {
            valido = false;
            insertar(data, "Error", "Cadena invalida");
            strcpy(Token,  ""); //limpiamos la variable
        }
    }

    if(valido)
    {
        insertar(data, "Id", data);
        strcpy(Token,  ""); //limpiamos la variable
        return true;
    }

    return false;
}

void insertar(char  nombre[], char tipo[], char lexema[])
{
    struct nodo *nuevo = NULL, *nAux = raiz;
    nuevo = malloc(sizeof(struct nodo));

    strcpy(nuevo->nombre, nombre);
    strcpy(nuevo->tipo, tipo);
    strcpy(nuevo->lexema, lexema);
    nuevo->anterior = NULL;
    nuevo->siguiente = NULL;

    if(raiz == NULL)
        raiz=nuevo;
    else
    {
        while(nAux->siguiente != NULL)
            nAux = nAux->siguiente;

        nuevo->anterior = nAux;
        nAux->siguiente = nuevo;
    }
}

void imprimirPre(struct nodo *reco)
{
    while (reco != NULL)
    {
        if(strcmp(reco->tipo, "Error") == 0)
        {
            Color(0,4);
            printf("  %s",reco->nombre);
            for(int i = strlen(reco->nombre); i < 22; i++)
            {
                printf(" ");
            }
            printf("%s",reco->tipo);
            for(int i = strlen(reco->tipo); i < 14; i++)
            {
                printf(" ");
            }
            printf("%s\n",reco->lexema);
            reco = reco->siguiente;
        }else{
            Color(0,15);
            printf("  %s",reco->nombre);
            for(int i = strlen(reco->nombre); i < 22; i++)
            {
                printf(" ");
            }
            printf("%s",reco->tipo);
            for(int i = strlen(reco->tipo); i < 14; i++)
            {
                printf(" ");
            }
            printf("%s\n",reco->lexema);
            reco = reco->siguiente;
        }
    }
}

void borrar(struct nodo *reco)
{
    if (reco != NULL)
    {
        borrar(reco->anterior);
        borrar(reco->siguiente);
        free(reco);
    }
}

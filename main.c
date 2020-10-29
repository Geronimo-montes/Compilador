#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <Windows.h>

#include "EstructuraToken.h"
#include "PalabraReservada.h"
#include "Identificador.h"
#include "Numeros.h"

#define numSimEspecial 20

//Variables
struct nodo *raiz = NULL;
char cadena[] = "";
char caracterAnterior;
bool auxCadena;
char simEspecial[numSimEspecial][3] = { "+", "-", "*", "/", "<", ">", "=", "&", "|", ";", ":", "(", ")", ">=", "<=", "!=", ":=","\"" };

//Funciones
struct Token crearToken(char nombre[], enum TipoToken tipo, char lexema[], double valor);
void identificarTooken();
void insertar(struct Token token);
void imprimirPre(struct nodo *reco);
void borrar(struct nodo *reco);
void Color(int Background, int Text);

int main()
{
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
                    identificarTooken();
                    simboloRaro = false;
                }
                break;

            case '!':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    identificarTooken();
                    simboloRaro = false;
                }
                break;

            case '>':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    identificarTooken();
                    simboloRaro = false;
                }
                break;

            case '<':
                if (caracterAnterior == '\0')
                {
                    caracterAnterior = aux[0];//Respaldamos el caracter leido
                    identificarTooken();
                    simboloRaro = false;
                }
                break;
            case '=':
                identificarTooken();

                if (caracterAnterior == ':')
                {
                    insertar(crearToken(":=", SimEsp, ":=", 0));
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '!')
                {
                    insertar(crearToken("!=", SimEsp, "!=", 0));
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '>')
                {
                    insertar(crearToken(">=", SimEsp, ">=", 0));
                    caracterAnterior = '\0';
                }
                else if(caracterAnterior == '<')
                {
                    insertar(crearToken("<=", SimEsp, "<=", 0));
                    caracterAnterior = '\0';
                }
                else
                {
                    insertar(crearToken("=", SimEsp, "=", 0));
                }

                simboloRaro = false;
                break;
            //Saltos de linea, tabulaciones y espacions son idicadores de insertar el
            //contenido de cadena
            case '\n':
            case '\t':
            case ' ':
                if(auxCadena == false)
                {
                    identificarTooken();
                }

                break;
            //En caso de no corresponder a nunguna opcion podemos asumir que se trata
            //de un simEsp simple
            case '"':
                if(cadena[0] == '\0')
                {
                    insertar(crearToken("\"", SimEsp, "\"", 0));
                    auxCadena = true;
                }
                else if (cadena[0] != '\0')
                {
                    insertar(crearToken(cadena, Cadena, cadena, 0));
                    insertar(crearToken("\"", SimEsp, "\"", 0));
                    strcpy(cadena,  "");
                    auxCadena = false;
                }
                break;
            default:
                if(caracterAnterior != '\0')
                {
                    //Puede que el caracter almacenado nunca se use por lo que hay que verificar
                    if(caracterAnterior != '!')
                    {
                        //el signo de exclamacion no es utilizado individualmente por lo que se descarta
                        insertar(crearToken(&caracterAnterior, SimEsp, &caracterAnterior, 0));
                    }
                    caracterAnterior = '\0';
                }

                for(int i = 0; i < numSimEspecial; i++)
                {
                    if(strcmp(aux, simEspecial[i]) == 0)
                    {
                        //Al encontrar un ocurrencia insertamos el nodo y rompemos el ciclo
                        identificarTooken();
                        insertar(crearToken(aux, SimEsp, aux, 0));
                        simboloRaro = false;
                        break;
                    }
                }

                break;
            }

            if(simbolo != ' ' && simbolo != '\n' && simbolo != '\t' && simbolo != '"' && auxCadena == false)
            {
                if(isalpha(simbolo) || isdigit(simbolo) || simbolo == '_' || simbolo == '.' || simboloRaro)
                {
                    strncat(cadena, aux,1);//Concatenamos
                }
            }
            else if (auxCadena == true && simbolo != '"')
            {

                strncat(cadena, aux,1);//Concatenamos

            }
        }
    }

    //al terminar de leer el docuemnto puede que token tenga un valor guardado
    identificarTooken();

    fclose(archivo);
    printf("  Nombre\t\tTipo\t\tLexema\t\tValor\n\n");
    imprimirPre(raiz);
    borrar(raiz);
    getch();
    return 0;
}


struct Token crearToken(char nombre[], enum TipoToken tipo, char lexema[], double valor)
{
    struct Token *nuevoToken = NULL;
    nuevoToken = malloc(sizeof(struct Token));

    strcpy(nuevoToken->nombre, nombre);
    nuevoToken->tipo = tipo;
    strcpy(nuevoToken->lexema, lexema);
    nuevoToken->valor = valor;

    return *nuevoToken;
}

/* Sirve para no repetir el mismo fracmento de codigo
 * ademas de que lo vuelve versatil a los cambios*/
void identificarTooken()
{



    if(strcmp(cadena, "\0") != 0)
    {
        if(identidacadorPalRes(cadena))
        {
            //La cadena analizada es una palRes
            insertar(crearToken(cadena, PalRes, cadena, 0));
            strcpy(cadena,  "");
        }
        else
        {
            switch (numeros(cadena))
            {
            case 1://Numero Valido
                insertar(crearToken(cadena, Num, cadena, atof(cadena)));
                strcpy(cadena,  "");
                break;

            case 2://Error lexico
                insertar(crearToken(cadena, Error, "Error Lexico. Numero no Valido.", atof(cadena)));
                strcpy(cadena,  "");
                break;

            case 3:
                if(identidacadorIdentificador(cadena))
                {
                    //La cadena es un identificador valido
                    insertar(crearToken(cadena, Id, cadena, 0));
                    strcpy(cadena,  "");
                }
                else  //Error lexico en el identidicador
                {
                    insertar(crearToken(cadena, Error, "Error Lexico. Identidicador no Valido.", 0));
                    strcpy(cadena,  "");
                }
                break;
            }
        }
    }


}

void insertar(struct Token token)
{
    struct nodo *nuevo = NULL, *nAux = raiz;
    nuevo = malloc(sizeof(struct nodo));

    nuevo->token = token;
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
        if(reco->token.tipo == 5)
            Color(0,4);
        else
            Color(0,15);

        printf("  %s", reco->token.nombre);
        for(int i = strlen(reco->token.nombre); i < 22; i++)
        {
            printf(" ");
        }
        printf("%s", tipoToken[reco->token.tipo]);
        for(int i = strlen(tipoToken[reco->token.tipo]); i < 14; i++)
        {
            printf(" ");
        }
        printf("%s", reco->token.lexema);
        for(int i = strlen(reco->token.lexema); i < 14; i++)
        {
            printf(" ");
        }
        printf("%.2f\n", reco->token.valor);
        reco = reco->siguiente;
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

void Color(int Background, int Text)  // Función para cambiar el color del fondo y/o pantalla
{
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos la consola.
    int New_Color= Text + (Background * 16); // Pero, para convertir los colores a un valor adecuado, se realiza el siguiente cálculo.
    SetConsoleTextAttribute(Console, New_Color); // Guardamos los cambios en la Consola.
}

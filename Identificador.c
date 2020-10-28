#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "Identificador.h"
/*Sacar numeros a un archivo particular*/
bool numeros(char data[])
{
    bool valido = true;

    if (isdigit(data[0]))
    {
        if(atoi(data) || atof(data))
        {
            valido = true;
        }else{
            //Token de error
            valido = false;
        }
    }else{
        valido = false;
    }
    
    return valido;
}

bool identidacadorIdentificador(char data[])
{
    bool valido = true;
    int i = 0;

    if(isalpha(data[0]) || data[0] == '_' )
    {
        while(data[i])
        {
            if(isalnum(data[i]) || data[i] == '_'){
                valido = true;
            }else{
                //Token de error en la variable
                valido = false;
                //insertar(data, "Error", "Cadena invalida");
                break;
            }

            i++;
        }
    }

    /*if(valido)
    {
        insertar(data, "Id", data);
        //strcpy(Token,  ""); //limpiamos la variable
        return true;
    }*/

    return valido;
}
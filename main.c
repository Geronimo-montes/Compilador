#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

struct nodo {
    int info;
    struct nodo *siguiente;
    struct nodo *anterior;
};

struct nodo *raiz=NULL;


void insertar(int x)
{
    struct nodo *nuevo = NULL, *nAux = raiz;
    nuevo = malloc(sizeof(struct nodo));
    nuevo->info = x;
    nuevo->anterior = NULL;
    nuevo->siguiente = NULL;
    //printf("%i", x);
    if(raiz == NULL){
        raiz=nuevo;
    }
    else
    {

    while(nAux->siguiente != NULL){
                nAux = nAux->siguiente;
            }
            nuevo->anterior = nAux;
            nAux->siguiente = nuevo;

    }
}

void imprimirPre(struct nodo *reco)
{
    while (reco != NULL)
    {
        printf("%i-",reco->info);
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


int main()
{
    insertar(100);
    insertar(50);
    insertar(25);
    insertar(75);
    insertar(150);

    printf("Lista: ");
    imprimirPre(raiz);

    borrar(raiz);
    getch();
    return 0;
}

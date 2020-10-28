#ifndef ESTRUCTURATOKEN_H_INCLUDED
#define ESTRUCTURATOKEN_H_INCLUDED

enum TipoToken{
    PalRes,
    Id,
    Num,
    SimEsp,
    Cadena,
    Error
};

char tipoToken[6][20] = {"PalRes", "Id", "Num", "SimEsp", "Cadena", "Error"};

struct Token{
    char nombre[80];
    enum TipoToken tipo;
    char lexema[80];
    int valor;
};

struct nodo
{
    struct Token token;
    struct nodo *siguiente;
    struct nodo *anterior;
};
#endif // ESTRUCTURATOKEN_H_INCLUDED

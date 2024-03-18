#include "bibliotecas.h"
#include "cabecalhos.h"
#include "structs.h"


//---------------------CONFIGURAÇÃO DAS SEÇÕES


int main()
{
    Album album;

    // mallocs
    album.figColadas = (Figurinha *)malloc(1 * sizeof(Figurinha));
    album.figFaltantes = (Figurinha *)malloc(1 * sizeof(Figurinha));
    album.figRepetidas = (Figurinha *)malloc(1 * sizeof(Figurinha));
    album.figDisponiveis = (Figurinha *)malloc(1 * sizeof(Figurinha));

    Figurinha Aleatorias[5];

    loginUsuario(&album);
    preencherFigurinhasAlbum(&album);
    menu(&album, Aleatorias);
    white();

    return 0;

} // main

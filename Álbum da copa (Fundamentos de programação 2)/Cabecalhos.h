#ifndef CABECALHOS_H
#define CABECALHOS_H
#include "structs.h"
#include "bibliotecas.h"

void preencherFigurinhasAlbum(Album *album);
void salvarFigurinhasAlbum(Album *album);
void menu(Album *album, Figurinha Aleatorias[]);
void funcaoInserir(Album *album);
void removerFigFaltantes(Album *album, char codigo[7]);
void adicionaFigFaltantes(Album *album, int indiceColadas, int indiceFaltantes);
void funcaoListar(Album *album);
void funcaoExcluir(Album *album);
void gerarIndiceAleatorio(Figurinha Aleatorias[]);
void salvarFigAleatorias(Figurinha Aleatorias[]);
void printarCodigoFigurinhasAleatorias(Figurinha Aleatorias[]);
void relacaoDisponiveisRepetidas(Album *album, Figurinha Aleatorias[]);
void comprarFigurinha(Album *album, Figurinha Aleatorias[]);
void loginUsuario(Album *album);
void folhearAlbum(Album *album);
void quiz(Album *album);

void black();
void red();
void green();
void yellow();
void blue();
void purple();
void cyan();
void white();
void Bblack();
void Bred();
void Bgreen();
void Byellow();
void Bblue();
void Bpurple();
void Bcyan();
void Bwhite(); 


void BRA_SC();
void CAN_SC();
void CHI_SC();
void UGD_SC();

#endif
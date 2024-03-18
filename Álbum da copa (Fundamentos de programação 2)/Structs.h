#ifndef STRUCTS_H
#define STRUCTS_H
typedef enum
{
    BRASIL = 1,
    CANADA,
    CHILE,
    UGANDA
} paises;

typedef enum
{
    SIM = 1,
    NAO
} escolha;

//###########################---Samuel---###################################

typedef struct
{
    char codigo[7];
    char titulo[35];
    char secao[29];
    char tipo[12];
    int indice;
} Figurinha;

//---------------------ESTRUTURA DO ÁLBUM

typedef struct
{
    // preciso mallocar esses vetores do tipo figurinha:
    Figurinha *figColadas;
    Figurinha *figFaltantes;
    Figurinha *figRepetidas;
    Figurinha *figDisponiveis;
    int qntColadas;
    int qntFaltantes;
    int qntRepetidas;
    int qntDisponiveis;
    double dinheiro_atual;
    float gastoPacote;
    int acertos_quiz;
    char nome[21];
    char senha[21];
    char telefone[21];
} Album;
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define tamanho_ram 500
#define max_stack 150

// Constates relativas ao opcode primario
#define addi 0b001110;
#define cmp 0b011111;
#define bc 0b010000;
#define mullw 0b011111;
#define b 0b010010 ;
#define sc 0b010001;

// Constantes relativas ao opcode extendido
#define cmpEXT 0b000000000;
#define mellwEXT 0b11101011; 


// o power pc pode ser configurado com 64 ou 32 bits, escolhemos a versao 32 bits
typedef struct xenon
{
    uint32_t r[32]; // registradores de proposito geral (r0 = v0, r1 = sp, r2 = tabela de conteudo de ponteiro, r3 - r10 registradores de argumento)
    uint32_t f[32]; // registradores de ponto flutuante
    uint32_t lr;    // link register (equivalente ao ra no mips)
    uint32_t ctr;   // count register(usado em loopings)
    uint32_t xer;   // registrador de excecao
    uint32_t fpscr; // registrador de excecao para ponto flutuante
    uint32_t cr;    // registrador de condicao (funciona como um vetor de tamanho 8)
    uint32_t mq;    // Multiply Quotient register

    uint32_t ram[tamanho_ram]; // a memoria do power pc contem as instrucoes e a stack
    uint32_t opcode, rs, rt, rd, shamt, funct;
    uint32_t pc; // program counter
    uint32_t ic; // instrucao atual
} XENON;

void inicializar_xenon(XENON *xenon)
{ // antes de executar o programa, os registradores sao setados em 0
    for (int i = 0; i < 32; i++)
    {
        xenon->r[i] = 0;
        xenon->f[i] = 0;
    }                        // for
    xenon->r[1] = max_stack; // a stack no power pc comeca em um valor maximo e vai decrescendo em direcao ao 0
    xenon->pc = 0;
    xenon->ic = 0;
} // fim funcao

int leArquivo(FILE* arq, XENON *xenon){
    int retorno;
    int i;
    do{
        i = fread(xenon->ram, tamanho_ram, 100, arq);
        retorno += i;
        printf("%d\n",i);
    }while(i != 0);

    return retorno;
}


int main()
{
    XENON xenon;
    FILE *arq;
    int nmrFuncoes;
    
    arq = fopen("arquivo.bin", "r+b");

    if(arq == NULL){
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        exit(1);
    }

    nmrFuncoes = leArquivo(arq, &xenon);

    printf("numeroo %d\n",nmrFuncoes);
    nmrFuncoes = nmrFuncoes/8;




    return EXIT_SUCCESS;
}
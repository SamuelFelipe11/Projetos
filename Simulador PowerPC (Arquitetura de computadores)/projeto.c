#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define tamanho_ram 500
#define max_stack 150

// Constates relativas ao opcode primario
#define addiPRI 0b001110  // addi (igual do mips)
#define cmpPRI 0b011111   // compara o valor de dois registradores e salva o resultado em cr
#define bcPRI 0b010000    // branch condicional
#define mullwPRI 0b011111 // multiplica dois registradores e salva em um terceiro
#define bPRI 0b010010     // branch (igual o j do mips)
#define scPRI 0b010001    // system call

// Constantes relativas ao opcode extendido
#define cmpEXT 0b000000000
#define mullwEXT 0b011101011

typedef enum
{
    EX_DIRETA = 0,
    EX_DETALHADA = 1
} EXECUCAO_PROGRAMA;

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

bool executar_programa(XENON *xenon, int tipo_execucao)
{
    int rt; // registrador de destino
    int ra; // registrador de source
    int si; // valor imediato do addi

    // opcodes primarios
    // faltou tratar o opcode

    switch (xenon->opcode)
    {
    case addiPRI:
        // decodificar ra, rt e si
        xenon->r[rt] = xenon->r[ra] + si;
        break;

    case mullwPRI:

        break;

    case bcPRI:

        break;

    case bPRI:

        break;

    case scPRI:

        break;

    default:
        return false;
        break;
    } // switch
} // fim funcao

int main()
{
    XENON xenon;

    // arquivo

    return EXIT_SUCCESS;
}
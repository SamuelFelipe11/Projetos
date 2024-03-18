#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


//==================================== TABELA HASH ============================================================

typedef struct cliente{
    char nome[50];
    int coordX;
    int coordY;
}CLIENTE;

typedef struct hashtable{
    int qtdeElem;
    CLIENTE *cliente;
}HashTable;


void inicializaHashTable(HashTable *t, int qtdeLinhas){
    t->qtdeElem=0;
    t->cliente = (CLIENTE*)malloc(qtdeLinhas*sizeof(CLIENTE));
}

bool estaCheia(HashTable *tabela, int qtdeLinhas){
    return(tabela->qtdeElem>=qtdeLinhas);
}

int funcaoHash(int num){
    return(num);
}

void insereElemHash(HashTable *tabela, CLIENTE cliente, int pos, int qtdeLinhas){
    if(estaCheia(tabela, qtdeLinhas)==true){
        printf("\n\nNao foi possivel inserir %s (Tabela Hash Cheia)\n\n", cliente.nome);
        return;
    }

    tabela->cliente[pos]=cliente;
    tabela->qtdeElem++;
}

int numero_linhas_arq(char nome_arq[])
{
    FILE *arq;
    char c;
    int linha = 0;

    arq = fopen(nome_arq, "r");

    if (arq == NULL)
    {
        printf("\nErro ao abrir o arquivo :(\n\n");
        return -1;
    } // if

    // pegar caracter por caracter do arquivo
    c = fgetc(arq);
    while (c != EOF)
    {
        if (c == '\n')
        {
            linha++;
        } // if
        // pegar caracter por caracter do arquivo
        c = fgetc(arq);
    }
    linha++;
    fclose(arq);
    return linha;
} // fim funcao



FILE* abreArquivo(char nomeArq[]){
    FILE* ponteiro;
    ponteiro = fopen(nomeArq, "r");

    if(ponteiro == NULL){
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        exit(1);
    }//if-else

    return ponteiro;
}

void leArquivo(FILE* coordClientes, HashTable *tabela, int qtdeLinhas){
    CLIENTE aux;
    int descartaLinha;
    char descartaLinha1[100];
    char descartaLinha2[100];

    
    fscanf(coordClientes,"%d, %[^,], %[^\n]\n", &descartaLinha, descartaLinha1, descartaLinha2);
    
    for(int i = 0; i < qtdeLinhas; i++){
        fscanf(coordClientes,"%[^,], %d, %d\n", aux.nome, &aux.coordX, &aux.coordY);
        insereElemHash(tabela, aux, i, qtdeLinhas);
    }

}//fim função


//========================================== GRAFOS ===================================================================
typedef struct grafoM{
    int  **matriz;
    int  vertices;
    bool digrafo; 
}GrafoMatriz;


void inicializa(GrafoMatriz *grafo, int N, bool eh_digrafo){
    grafo->vertices = N;
    grafo->digrafo = eh_digrafo;

    grafo->matriz=(int**) malloc(N*sizeof(int*));
    for(int i=0; i<N; i++){
         grafo->matriz[i]= (int*) malloc(N*sizeof(int));
         for(int j=0; j<N; j++){
            grafo->matriz[i][j]=0;
         }
    }
}

void destroiGrafo(GrafoMatriz *grafo){
    for(int i=0; i<grafo->vertices; i++)
        free(grafo->matriz[i]);
    free(grafo->matriz);
}

void conectaAresta(GrafoMatriz *grafo, int v1, int v2, int peso){
    grafo->matriz[v1][v2]=peso;
    if(grafo->digrafo==false)  grafo->matriz[v2][v1]=peso;
}


double distanciaEntrePontos(int x1, int y1, int x2, int y2){

    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void leArqAdjacencia(HashTable *tabela){
    FILE *arq = abreArquivo("clientes adjacencia.csv");
    int qtdeLinhas = numero_linhas_arq("clientes adjacencia.csv");
    printf("LINHAS - %d\n", qtdeLinhas);
    
    char linha[5000];
    fgets(linha, 5000, arq);
    
    
    int ligacao = 0;
    double peso = 0;
    for(int i = 0; i < qtdeLinhas-1; i++){
        fscanf(arq, "%[^,],", linha);
        
            
        for(int j = 0; j < qtdeLinhas-1; j++){
            if(j != qtdeLinhas-1) fscanf(arq, "%d,", &ligacao);
            else fscanf(arq,"%d", &ligacao);


            if(ligacao == 1){
                peso = distanciaEntrePontos(tabela->cliente[i].coordX, tabela->cliente[i].coordY, tabela->cliente[j].coordX, tabela->cliente[i].coordY);
                printf("%d - %d - %d - %d\n", tabela->cliente[i].coordX, tabela->cliente[i].coordY, tabela->cliente[j].coordX, tabela->cliente[i].coordY);
                printf("\n\nPESO ENTRE %s E %s = %lf\n\n",tabela->cliente[i].nome, tabela->cliente[j].nome, peso);
            }
            

        }//for
        printf("\n");
        fscanf(arq, "\n");

    }//for



    fclose(arq);

}//fim função




//==================================================================================================================
int main(){
    int qtdeLinhas;
    HashTable tabela;
    FILE* coordenadas_clientes;
    coordenadas_clientes = abreArquivo("coordenadas_clientes.csv");

    qtdeLinhas = numero_linhas_arq("coordenadas_clientes.csv"); 

    inicializaHashTable(&tabela, qtdeLinhas);
    leArquivo(coordenadas_clientes, &tabela, qtdeLinhas);

    leArqAdjacencia(&tabela);


    fclose(coordenadas_clientes);
    return EXIT_SUCCESS;
}
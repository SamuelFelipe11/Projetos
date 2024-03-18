#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


//========================================================================== STRUCTS REFERENTES AO CADASTRO ================================================================================================================================================================

typedef struct cadastros{
    char id[100];
    int diaNasci;
    int mesNasci;
    int anoNasci;
    char saude;

    struct cadastros* prox;
    struct cadastros* ant;
}CADASTROS;

typedef struct lista{
    int qtdeElem;
    CADASTROS* ini;
}LISTA;

//======================================================================== STRUCTS REFERENTE A FILA DINAMICA ==================================================================================================================================================================

typedef struct pessoa{
    char nome[50];
    struct pessoa *prox;
}FILA;

typedef struct filaDinamica {
    FILA *ini;
    FILA *fim;
    int qtdeElem;
}FILADINAMICA;


//========================================================================= STRUCTS REFERENTES AS FILAS ESTATICAS (BUFFET) ================================================================================================================================================================

typedef struct buffet{
    char nome[100];
}BUFFET;

typedef struct filaEstaticaD{
    BUFFET fila[15];
    int ini;
    int fim;
    int qtdeElem;
}DIABETICOS;

typedef struct filaEstaticaH{
    BUFFET fila[15];
    int ini;
    int fim;
    int qtdeElem;
}HIPERTENSOS;

typedef struct filaEstaticaN{
    BUFFET fila[15];
    int ini;
    int fim;
    int qtdeElem;
}NORMAIS;


//========================================================================== CABEÇALHO ================================================================================================================================================================
void limpaTela();
int contaLinhas(char *arquivo);
void inicializaLista(LISTA* l);
bool estaVaziaCadastros(LISTA* l);
char* menorElemLista(LISTA* l);
bool insereElemLista(LISTA* l, CADASTROS *novo);
void imprimeLista(LISTA* l);
void destroiLista(LISTA* l);
void removeElemLista(LISTA* l, char id[]);
CADASTROS* pesquisaElemLista(LISTA* l, char id[]);
void recebeDadosClientes(LISTA *l);
void recebeNome(LISTA *l);
CADASTROS* pesquisaNome(LISTA *l);
void alterarCadastro(LISTA *l);
void inicializaFilaDinamica(FILADINAMICA *f);
bool estaVaziaFila(FILADINAMICA *f);
int qtdeElementos(FILADINAMICA *f);
bool estaVazia(FILADINAMICA *f);
void insereElemFila(FILADINAMICA *f, char nome[]);
bool verificaIdade(int dia, int mes, int ano);
void montaFilaEspera(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n);
void inicializaFilaD(DIABETICOS *f);
void inicializaFilaH(HIPERTENSOS *f);
void inicializaFilaN(NORMAIS *f);
bool estaVaziaD(DIABETICOS *f);
bool estaCheiaD(DIABETICOS *f);
bool estaVaziaH(HIPERTENSOS *f);
bool estaCheiaH(HIPERTENSOS *f);
bool estaVaziaN(NORMAIS *f);
bool estaCheiaN(NORMAIS *f);
bool insereElemFilaD(DIABETICOS *f, char nome[]);
bool insereElemFilaH(HIPERTENSOS *f, char nome[]);
bool insereElemFilaN(NORMAIS *f, char nome[]);
void imprimeFilaD(DIABETICOS *f);
void imprimeFilaH(HIPERTENSOS *f);
void imprimeFilaN(NORMAIS *f);
void selecionaGer(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n);
void gerenciamentoFilas(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n);
void gerenciamentoCadastros(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n);


//=============================================================================================================================================================================================================================================================================


//=================================================================================== FUNCOES BASICAS ================================================================================================================================================================

typedef struct{
    int dia;
    int mes;
    int ano;
}DATA_ATUAL;

void limpaTela(){
    system("cls");
}//fim função

DATA_ATUAL obtemdata(void){
    DATA_ATUAL data;
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    data.dia = tm.tm_mday;
    data.mes = tm.tm_mon + 1;
    data.ano = tm.tm_year + 1900;

    return data;
}//fim função


int contaLinhas(char *arquivo) {
    FILE *leArquivo;
    char ch;
    int contador = 0;

    leArquivo = fopen(arquivo, "r");

    if (leArquivo == NULL) {
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        exit(1);
    }

    while ((ch = fgetc(leArquivo)) != EOF) {
        if (ch == '\n') {
            contador++;
        }//if-else
    }//while

    fclose(leArquivo);

    return contador;
}//fim funcao

//============================================================================== FUNCOES REFERENTES A LISTA DUPLAMENTE ENCADEADA ==========================================================================================================================================================

void inicializaLista(LISTA* l){
    l->qtdeElem = 0;
    l->ini = NULL;
}//fim função

bool estaVaziaCadastros(LISTA* l){
    return(l->qtdeElem == 0);
}//fim função

char* menorElemLista(LISTA* l){
    return l->ini->id;
}//fim função

bool insereElemLista(LISTA* l, CADASTROS *novo){
    CADASTROS *aux = l->ini;

    if(estaVaziaCadastros(l) == true){
        l->ini = novo;
        l->ini->ant = NULL;
        l->ini->prox = NULL;
        l->qtdeElem++;
        return true;
    }
 
    if(strcasecmp(l->ini->id, novo->id) == 1){

        novo->prox = l->ini;
        l->ini->ant = novo;
        l->ini = novo;
        l->qtdeElem++;

        return true;
    }
    
    while(aux->prox && strcasecmp(aux->prox->id, novo->id) == -1 ){
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    if(aux->prox) aux->prox->ant = novo;
    novo->ant = aux;
    aux->prox = novo;

    l->qtdeElem++;
    return true;

}//fim função

void imprimeLista(LISTA* l){
    int cont = 0;
    printf("\nElementos da lista:\n");
    CADASTROS *aux = l->ini;

    while(aux != NULL){
        printf("\n[%d]id = %s", ++cont, aux->id);
        aux = aux->prox;
    }

    printf("\n\n");
}//fim função


void destroiLista(LISTA* l){
    CADASTROS *aux = l->ini;

    while(aux!=NULL){
        l->ini = aux->prox;
        free(aux);
        aux = l->ini;
    }

}//fim função

void removeElemLista(LISTA* l, char id[]){
    CADASTROS* aux = l->ini;
    CADASTROS* remove = NULL;

    if(strcasecmp(aux->id, id) == 0){
        l->ini = aux->prox;
        l->ini->ant = NULL;
        free(aux);
        return;
    }

    while(aux->prox!=NULL && strcasecmp(aux->prox->id, id) != 0) aux = aux->prox;
     
    if(aux->prox == NULL){
        printf("\n==> O ELEMENTO PROCURADO NAO ESTA NA LISTA\n");
        return;
    }

    remove = aux->prox;
    aux->prox = remove->prox;
    if(aux->prox) aux->prox->ant = aux;

    free(remove);
    l->qtdeElem--;

}//fim função

CADASTROS* pesquisaElemLista(LISTA* l, char id[]){
    CADASTROS *aux = l->ini, *retorno = NULL;

    while(aux && strcasecmp(aux->id, id) != 0) aux = aux->prox; 


    if(aux) retorno = aux;
    
    
    return retorno;
}//fim função


void recebeDadosClientes(LISTA *l){

    FILE* arquivoClientes = fopen("DadosClientes - Pagina1.csv", "r");
    int linhas = contaLinhas("DadosClientes - Pagina1.csv");
    char descartaLinha[100];
    char descartaLinha1[100];
    char descartaLinha2[100];
    CADASTROS *aux;

    aux = (CADASTROS*)malloc(sizeof(CADASTROS));

    if(arquivoClientes == NULL){
        printf("ERRO NA ABERTURA DO ARQUIVO\n\n");
        exit(1);
    }

    fscanf(arquivoClientes, "%[^,],%[^,],%[^\n]\n", descartaLinha, descartaLinha1, descartaLinha2);

    for(int i = 0; i < linhas; i++){
        aux = (CADASTROS*)malloc(sizeof(CADASTROS));
        fscanf(arquivoClientes, "%[^,],%d/%d/%d,%c\n", aux->id, &aux->diaNasci, &aux->mesNasci, &aux->anoNasci, &aux->saude);
        insereElemLista(l, aux);
    }

    fclose(arquivoClientes);

}//fim funcao

void recebeNome(LISTA *l){
    CADASTROS *aux = NULL;

    aux = (CADASTROS*)malloc(sizeof(CADASTROS));

    setbuf(stdin, NULL);
    printf("Digite seu nome completo:");
    fgets(aux->id, 100, stdin);
    aux->id[strcspn(aux->id, "\n")] = '\0';
    setbuf(stdin, NULL);

    setbuf(stdin, NULL);
    printf("Digite sua data de nascimento (DD/MM/AA):");
    scanf("%d/%d/%d", &aux->diaNasci, &aux->mesNasci, &aux->anoNasci);

    setbuf(stdin, NULL);
    printf("Digite sua condicao de saude\n");
    printf("OBS: (H) Hipertenso / (D) Diabetico / (N) Normal\n");
    scanf("%c", &aux->saude);

    insereElemLista(l, aux);

    printf("Cadastro realizado com sucesso!\n");

    imprimeLista(l);

}//fim funcao

CADASTROS* pesquisaNome(LISTA *l){
    char nome[100];
    CADASTROS *retorno;

    printf("Digite seu nome completo:");
    fgets(nome, 100, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    setbuf(stdin, NULL);

    retorno = pesquisaElemLista(l, nome);


    if(retorno == NULL){
        printf("Voce ainda nao tem cadastro\n\n");
    }else{
        printf("\n\nNome: %s\n", retorno->id);
        printf("Nasc: %d/%d/%d\n", retorno->diaNasci, retorno->mesNasci, retorno->anoNasci);
        switch(retorno->saude){
            case 'H':
            case 'h':

                    printf("Condicao: Hipertenso\n\n");
                    break;
            
            case 'D':
            case 'd':

                    printf("Condicao: Diabetico\n\n");
                    break;

            default:

                    printf("Condicao: Normal\n\n");
                    break;
        }//switch
    }//if-else

    return retorno;

}//fim funcao

void alterarCadastro(LISTA *l){
    int opcao;
    CADASTROS *retorno;
    retorno = pesquisaNome(l);
    
    printf("\n\nQual informacao deseja alterar?\n1 - Nome\n2 - Nascimento\n3 - Condicao\n");
    scanf("%d", &opcao);

    if(opcao == 1){
        setbuf(stdin, NULL);
        printf("Novo nome:");
        fgets(retorno->id, 100, stdin);
        retorno->id[strcspn(retorno->id, "\n")] = '\0';
        printf("\n\n");
    }else if(opcao == 2){
        setbuf(stdin, NULL);
        printf("Nova data de nascimento (DD/MM/AA):");
        scanf("%d/%d/%d", &retorno->diaNasci, &retorno->mesNasci, &retorno->anoNasci);
        printf("\n\n");
    }else if(opcao == 3){
        setbuf(stdin, NULL);
        printf("Nova condicao:");
        scanf("%c", &retorno->saude);
        printf("\n\n");
    }else{
        setbuf(stdin, NULL);
        printf("OPCAO INVALIDA, TENTE NOVAMENTE\n\n");
        alterarCadastro(l);
    }
}//fim funcao

//============================================================================== FUNCOES REFERENTES A FILA DINAMICA (ESPERA) =====================================================================================================================================

void inicializaFilaDinamica(FILADINAMICA *f){
    f->qtdeElem=0;
    f->fim = f->ini = NULL;
}

bool estaVaziaFila(FILADINAMICA *f){
    return(f->qtdeElem==0);
}

int qtdeElementos(FILADINAMICA *f){
    return(f->qtdeElem);
}

bool estaVazia(FILADINAMICA *f){
    return(f->qtdeElem==0);
}

void insereElemFila(FILADINAMICA *f, char nome[]){
    FILA *aux = NULL;
    aux = (FILA*) malloc(sizeof(FILENAME_MAX));
    strcpy(aux->nome, nome);
    aux->prox = NULL;

    if(estaVazia(f)==true){
        f->ini=f->fim=aux;
    }
    else{
        f->fim->prox = aux;
        f->fim = aux;
    }
    f->qtdeElem++;    
}

bool verificaIdade(int dia, int mes, int ano){
    DATA_ATUAL Data;
    Data = obtemdata();

    if(ano <= (Data.ano - 12)){
        if(mes <= Data.mes){
            if(dia < Data.dia){
                return false;        
            }  
        }  
    } 

    return true;
}//fim função


void montaFilaEspera(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n){
    char nome[100];
    int linhas = contaLinhas("DadosChegada.txt");
    CADASTROS* retorno = NULL;
    FILE* arquivoChegada = fopen("DadosChegada.txt", "r");

    if(arquivoChegada == NULL){
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        exit(1);
    }//if-else

    for(int i = 0; i < linhas; i++){
        fscanf(arquivoChegada, "%[^\n]\n", &nome);
        retorno = pesquisaElemLista(l, nome);
        if(retorno != NULL){
            if((verificaIdade(retorno->diaNasci, retorno->mesNasci, retorno->anoNasci)) == true){
                printf("entra %d\n", i+1);
            }else{
                printf("nao entra %d\n", i+1);
            }  
            if((verificaIdade(retorno->diaNasci, retorno->mesNasci, retorno->anoNasci)) == true){

                if(insereElemFilaD(d, nome) == false){
                    if(insereElemFilaH(h, nome) == false){
                        if(insereElemFilaN(n, nome) == false){
                            insereElemFila(f, nome);
                        }//if-else
                    }//if-else
                }//if-else               
            }//if-else
        }//if-else
    }//for

    fclose(arquivoChegada);

}//fim funcao

//========================================================================== FUNCOES REFERENTES A FILA ESTATICA (BUFFETS) =========================================================================================================================================

void inicializaFilaD(DIABETICOS *f){
    f->ini = f->fim =-1;
    f->qtdeElem =0;
}

void inicializaFilaH(HIPERTENSOS *f){
    f->ini = f->fim =-1;
    f->qtdeElem =0;
}

void inicializaFilaN(NORMAIS *f){
    f->ini = f->fim =-1;
    f->qtdeElem =0;
}

bool estaVaziaD(DIABETICOS *f){
    return(f->qtdeElem==0);
}

bool estaCheiaD(DIABETICOS *f){
    return(f->qtdeElem==15);
}

bool estaVaziaH(HIPERTENSOS *f){
    return(f->qtdeElem==0);
}

bool estaCheiaH(HIPERTENSOS *f){
    return(f->qtdeElem==15);
}

bool estaVaziaN(NORMAIS *f){
    return(f->qtdeElem==0);
}

bool estaCheiaN(NORMAIS *f){
    return(f->qtdeElem==15);
}


bool insereElemFilaD(DIABETICOS *f, char nome[]){
    if(estaCheiaD(f)==true){
        return false;
    }

    if(estaVaziaD(f)==true){
        f->fim = f->ini = 0;
        strcpy(f->fila[0].nome, nome);
        f->qtdeElem++;
        return true;
    }

    f->fim=(f->fim+1)%15;
    strcpy(f->fila[f->fim].nome, nome);
    f->qtdeElem++;
    return true;

}

bool insereElemFilaH(HIPERTENSOS *f, char nome[]){
    if(estaCheiaH(f)==true){
        return false;
    }

    if(estaVaziaH(f)==true){
        f->fim = f->ini = 0;
        strcpy(f->fila[0].nome, nome);
        f->qtdeElem++;
        return true;
    }

    f->fim=(f->fim+1)%15;
    strcpy(f->fila[f->fim].nome, nome);
    f->qtdeElem++;
    return true;

}

bool insereElemFilaN(NORMAIS *f, char nome[]){
    if(estaCheiaN(f)==true){
        return false;
    }

    if(estaVaziaN(f)==true){
        f->fim = f->ini = 0;
        strcpy(f->fila[0].nome, nome);
        f->qtdeElem++;
        return true;
    }

    f->fim=(f->fim+1)%15;
    strcpy(f->fila[f->fim].nome, nome);
    f->qtdeElem++;
    return true;

}

void imprimeFilaD(DIABETICOS *f){
    int pos=f->ini;
    for(int i=1; i<=f->qtdeElem; i++){
        printf("\n %s", i, f->fila[pos].nome);
        pos=(pos+1)%15;
    }

    printf("\n\n");
}

void imprimeFilaH(HIPERTENSOS *f){
    int pos=f->ini;
    for(int i=1; i<=f->qtdeElem; i++){
        printf("\n[%d] %s", i, f->fila[pos].nome);
        pos=(pos+1)%15;
    }

    printf("\n\n");
}

void imprimeFilaN(NORMAIS *f){
    int pos=f->ini;
    for(int i=1; i<=f->qtdeElem; i++){
        printf("\n[%d] %s", i, f->fila[pos].nome);
        pos=(pos+1)%15;
    }

    printf("\n\n");
}
//============================================================================================================================================================

//============================================================== MENUS =============================================================================================================

void selecionaGer(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n){
    int opcao;
    printf("Bem-vindo(a) ao gerenciador do restaurante Laços de Comida! O que desejas fazer?\n\n");
    printf("1 - Gerenciador de cadastros\n2 - Gerenciador de filas\n\n");
    
    do{
        setbuf(stdin, NULL);
        scanf("%d", &opcao);
    }while(opcao != 1 && opcao != 2);
    
    if(opcao == 1){
        limpaTela(); 
        gerenciamentoCadastros(l, f, d, h, n);
    }else{
        limpaTela(); 
        gerenciamentoFilas(l, f, d, h, n);
    }
}//fim funcao

void gerenciamentoFilas(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n){
    
    int opcao;
    while(1){
    printf("\n1 - Montar fila de espera\n2 - Proximo a entrar\n3 - Sair do restaurante\n4 - Imprimir fila D\n5 - Imprimir fila H\n6 - Imprimir fila N\n8 - Imprimir fila de espera\n9 - Sair");
    scanf("%d", &opcao);
    setbuf(stdin, NULL);
    
        switch(opcao){
            
            case 1:
                limpaTela();
                montaFilaEspera(l, f, d, h, n);
                printf("Fila organizada com sucesso!");
                break;
            
            case 2:
            
                break;
                
            case 3:
            
                break;
                
            case 4:
                limpaTela();
                imprimeFilaD(d);
                sleep(10);
                break;
                
            case 5:
                limpaTela();
                imprimeFilaH(h);
                sleep(10);
                break;
                
            case 6:
                limpaTela();
                imprimeFilaN(n);
                sleep(10);
                break;
                
            case 7:
            
                break;
                
            case 8:
            
                break;
                
            case 9:
                selecionaGer(l, f, d, h, n);
                break;
                
            default:
                printf("Opcao invalida, por favor, tente novamente:");
                setbuf(stdin, NULL);
                scanf("%d", &opcao);
                break;
            
        }//switch
    }//while
    
}//fim função

void gerenciamentoCadastros(LISTA* l, FILADINAMICA* f, DIABETICOS* d, HIPERTENSOS* h, NORMAIS* n)
{
    int opcao;
    

    while(1){

    setbuf(stdin, NULL);
    printf("1 - Realizar um novo cadastro\n2 - Consultar um cadastro\n3 - Alterar um cadastro\n4 - Sair\n");
    scanf("%d", &opcao);
        
        switch(opcao){
            
            case 1:
                setbuf(stdin, NULL);
                limpaTela(); 
                recebeNome(l);
                break;
            
            case 2:
                setbuf(stdin, NULL);
                limpaTela(); 
                pesquisaNome(l);
                break;
                
            case 3:
                setbuf(stdin, NULL);
                limpaTela();
                alterarCadastro(l);
                break;
                
            case 4:
                limpaTela();
                selecionaGer(l, f, d, h, n);

                break;
                
            default:
                printf("Opcao invalida, por favor, tente novamente:");
                setbuf(stdin, NULL);
                scanf("%d", &opcao);
                break;
            
        }//switch
    }//while
}//fim função

//===========================================================================================================================================================================

int main(){
    LISTA l;
    FILADINAMICA f;
    DIABETICOS d;
    HIPERTENSOS h;
    NORMAIS n;
    //CADASTROS* retorno;

    inicializaLista(&l);
    inicializaFilaDinamica(&f);
    inicializaFilaD(&d);
    inicializaFilaH(&h);
    inicializaFilaN(&n);
    recebeDadosClientes(&l);
    //imprimeLista(&l);
    selecionaGer(&l, &f, &d, &h, &n);


    destroiLista(&l);

    return EXIT_SUCCESS;
}
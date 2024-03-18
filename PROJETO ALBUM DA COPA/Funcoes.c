#include "cabecalhos.h"
#include "bibliotecas.h"
#include "structs.h"

void black() { printf("\033[0;30m"); }
void red() { printf("\033[0;31m"); }
void green() { printf("\033[0;32m"); }
void yellow() { printf("\033[0;33m"); }
void blue() { printf("\033[0;34m"); }
void purple() { printf("\033[0;35m"); }
void cyan() { printf("\033[0;36m"); }
void white() { printf("\033[0;37m"); }
void Bblack() { printf("\033[1;30m"); }
void Bred() { printf("\033[1;31m"); }
void Bgreen() { printf("\033[1;32m"); }
void Byellow() { printf("\033[1;33m"); }
void Bblue() { printf("\033[1;34m"); }
void Bpurple() { printf("\033[1;35m"); }
void Bcyan() { printf("\033[1;36m"); }
void Bwhite() { printf("\033[1;37m"); }
void loginUsuario(Album *album)
{
    FILE *arqSalvaUsuario = fopen("ArqSalvaUsuario.dat", "rb");
    int chances = 10;

    // verificar a existencia do arquivo salva usuario
    if (arqSalvaUsuario == NULL)
    {
        // criar arquivo caso  ele nao exista
        arqSalvaUsuario = fopen("ArqSalvaUsuario.dat", "wb");

        printf("Digite seu nome (este sera seu usuario de acesso ao sistema)\n");
        fgets(album->nome, 20, stdin);
        album->nome[strcspn(album->nome, "\n")] = '\0';
        setbuf(stdin, NULL);

        printf("Digite seu telefone\n");
        fgets(album->telefone, 20, stdin);
        album->telefone[strcspn(album->telefone, "\n")] = '\0';
        setbuf(stdin, NULL);

        printf("Agora, defina sua senha\n");
        fgets(album->senha, 20, stdin);
        album->senha[strcspn(album->senha, "\n")] = '\0';
        setbuf(stdin, NULL);

        // primeira run
        album->dinheiro_atual = 50;
        album->gastoPacote = 0;
        album->acertos_quiz = 0;

        fwrite(album->nome, sizeof(char), 21, arqSalvaUsuario);
        fwrite(album->senha, sizeof(char), 21, arqSalvaUsuario);
        fwrite(album->telefone, sizeof(char), 21, arqSalvaUsuario);
        fwrite(&album->dinheiro_atual, sizeof(double), 1, arqSalvaUsuario);
        fwrite(&album->gastoPacote, sizeof(float), 1, arqSalvaUsuario);
        fwrite(&album->acertos_quiz, sizeof(int), 1, arqSalvaUsuario);

        fclose(arqSalvaUsuario);
    }
    else
    { // se o  arquivo existe
        char usuario[21];
        char senha[21];
        // obter os dados do aquivo salva  usuario
        fread(album->nome, sizeof(char), 21, arqSalvaUsuario);
        fread(album->senha, sizeof(char), 21, arqSalvaUsuario);
        fread(album->telefone, sizeof(char), 21, arqSalvaUsuario);
        fread(&album->dinheiro_atual, sizeof(double), 1, arqSalvaUsuario);
        fread(&album->gastoPacote, sizeof(float), 1, arqSalvaUsuario);
        fread(&album->acertos_quiz, sizeof(int), 1, arqSalvaUsuario);
        do
        {
            system("clear");
            printf("Digite seu usuário e senha (%d tentativas)\n", chances);

            if (chances == 0)
            {
                Bred();
                printf("LIMITE DE TENTATIVAS EXCEDIDO\nENCERRANDO O PROGRAMA\n");
                remove("ArqSalvaUsuario.dat");
                remove("ArqSalvaFig.dat");
                exit(1);
            } // if-else

            printf("Usuario:");
            fgets(usuario, 20, stdin);
            usuario[strcspn(usuario, "\n")] = '\0';
            setbuf(stdin, NULL);

            printf("Senha:");
            fgets(senha, 20, stdin);
            senha[strcspn(senha, "\n")] = '\0';
            setbuf(stdin, NULL);

            chances--;
        } while (strcasecmp(usuario, album->nome) != 0 || strcmp(senha, album->senha) != 0);
        fclose(arqSalvaUsuario);
    } // else
} // função que realiza o login do usuário

void preencherFigurinhasAlbum(Album *album)
{
    FILE *arqSalvaFig;
    arqSalvaFig = fopen("ArqSalvaFig.dat", "rb");

    // se o arquivo nao existe, vou cria-lo
    if (arqSalvaFig == NULL)
    {
        arqSalvaFig = fopen("ArqSalvaFig.dat", "wb");
        FILE *arqFigTotais = fopen("figTotais.csv", "r");

        // NA primiera vez q o programa for aberto todas as figurinhas do usuario serao faltantes
        album->qntColadas = 0;
        album->qntFaltantes = 678;
        album->qntRepetidas = 0;
        album->qntDisponiveis = 0;

        // salvar informacoes no arquivo salva figurinha
        fwrite(&album->qntColadas, sizeof(int), 1, arqSalvaFig);
        fwrite(&album->qntFaltantes, sizeof(int), 1, arqSalvaFig);
        fwrite(&album->qntRepetidas, sizeof(int), 1, arqSalvaFig);
        fwrite(&album->qntDisponiveis, sizeof(int), 1, arqSalvaFig);

        album->figFaltantes = (Figurinha *)realloc(album->figFaltantes, album->qntFaltantes * sizeof(Figurinha));

        // obter as figurinhas faltantes
        fseek(arqFigTotais, sizeof(char) * 22, SEEK_SET);
        for (int i = 0; i < 678; i++)
        {
            fscanf(arqFigTotais, "%[^,],%[^,],%[^,],%[^\n]\n", album->figFaltantes[i].codigo, album->figFaltantes[i].titulo, album->figFaltantes[i].secao, album->figFaltantes[i].tipo);
            album->figFaltantes[i].indice = i + 1;
        } // for

        // escrever figurinhas obtidas no arquivo;
        fwrite(album->figColadas, sizeof(Figurinha), album->qntColadas, arqSalvaFig);
        fwrite(album->figFaltantes, sizeof(Figurinha), album->qntFaltantes, arqSalvaFig);
        fwrite(album->figRepetidas, sizeof(Figurinha), album->qntRepetidas, arqSalvaFig);
        fwrite(album->figDisponiveis, sizeof(Figurinha), album->qntDisponiveis, arqSalvaFig);

        fclose(arqSalvaFig);
        fclose(arqFigTotais);
    }
    else
    { // se o arquivo existir, ler as informacoes
        fread(&album->qntColadas, sizeof(int), 1, arqSalvaFig);
        fread(&album->qntFaltantes, sizeof(int), 1, arqSalvaFig);
        fread(&album->qntRepetidas, sizeof(int), 1, arqSalvaFig);
        fread(&album->qntDisponiveis, sizeof(int), 1, arqSalvaFig);

        // realocar as quantidades
        if (album->qntColadas != 0)
        {
            album->figColadas = (Figurinha *)realloc(album->figColadas, album->qntColadas * sizeof(Figurinha));
        } // if

        if (album->qntFaltantes != 0)
        {
            album->figFaltantes = (Figurinha *)realloc(album->figFaltantes, album->qntFaltantes * sizeof(Figurinha));
        } // if

        if (album->qntRepetidas != 0)
        {
            album->figRepetidas = (Figurinha *)realloc(album->figRepetidas, album->qntRepetidas * sizeof(Figurinha));
        } // if

        if (album->qntDisponiveis != 0)
        {
            album->figDisponiveis = (Figurinha *)realloc(album->figDisponiveis, album->qntDisponiveis * sizeof(Figurinha));
        } // if

        // ler structs
        fread(album->figColadas, sizeof(Figurinha), album->qntColadas, arqSalvaFig);
        fread(album->figFaltantes, sizeof(Figurinha), album->qntFaltantes, arqSalvaFig);
        fread(album->figRepetidas, sizeof(Figurinha), album->qntRepetidas, arqSalvaFig);
        fread(album->figDisponiveis, sizeof(Figurinha), album->qntDisponiveis, arqSalvaFig);
        fclose(arqSalvaFig);
    } // else
} // fim funcao

void salvarFigurinhasAlbum(Album *album)
{
    FILE *arqSalvaFig = fopen("ArqSalvaFig.dat", "wb");
    FILE *arqSalvaUsuario = fopen("ArqSalvaUsuario.dat", "wb");

    // salvar figurinhas
    fwrite(&album->qntColadas, sizeof(int), 1, arqSalvaFig);
    fwrite(&album->qntFaltantes, sizeof(int), 1, arqSalvaFig);
    fwrite(&album->qntRepetidas, sizeof(int), 1, arqSalvaFig);
    fwrite(&album->qntDisponiveis, sizeof(int), 1, arqSalvaFig);

    fwrite(album->figColadas, sizeof(Figurinha), album->qntColadas, arqSalvaFig);
    fwrite(album->figFaltantes, sizeof(Figurinha), album->qntFaltantes, arqSalvaFig);
    fwrite(album->figRepetidas, sizeof(Figurinha), album->qntRepetidas, arqSalvaFig);
    fwrite(album->figDisponiveis, sizeof(Figurinha), album->qntDisponiveis, arqSalvaFig);

    fclose(arqSalvaFig);

    // salvar informacoes do usuario
    fwrite(album->nome, sizeof(char), 21, arqSalvaUsuario);
    fwrite(album->senha, sizeof(char), 21, arqSalvaUsuario);
    fwrite(album->telefone, sizeof(char), 21, arqSalvaUsuario);
    fwrite(&album->dinheiro_atual, sizeof(double), 1, arqSalvaUsuario);
    fwrite(&album->gastoPacote, sizeof(float), 1, arqSalvaUsuario);
    fwrite(&album->acertos_quiz, sizeof(int), 1, arqSalvaUsuario);

    fclose(arqSalvaUsuario);
} // fim funcao

//---------------------FUNÇÕES INSERIR, LISTAR E EXCLUIR

void removerFigFaltantes(Album *album, char codigo[7])
{
    int salvaFaltantes;

    for (int i = 0; i < album->qntFaltantes; i++)
    {
        if (strcasecmp(album->figFaltantes[i].codigo, codigo) == 0)
        {
            salvaFaltantes = i;
        } // if-else faltantes
    }

    for (int j = salvaFaltantes; j < album->qntFaltantes; j++)
    {

        album->figFaltantes[j] = album->figFaltantes[j + 1];
    } // for

    album->qntFaltantes--;
    album->figFaltantes = (Figurinha *)realloc(album->figFaltantes, album->qntFaltantes * sizeof(Figurinha));
}

void funcaoInserir(Album *album)
{
    int contador = 0;
    int salvaIndice;
    int salvaIndiceColadas = 0;
    char codigoDigitado[10];

    while (2 > 1)
    {
        Bwhite();
        contador = 0;
        salvaIndice = -1;

        // sai da função se o jogador não possui nenhuma figurinha para ser colada
        if (album->qntDisponiveis == 0)
        {
            Bred();
            printf("ERRO: você não possui nenhuma figurinha para ser colada\n");
            sleep(1);
            break;
        }

        printf("\nFIGURINHAS DISPONIVEIS:\n");

        for (int i = 0; i < album->qntDisponiveis; i++)
        {
            printf("|%s|-|%s|\n", album->figDisponiveis[i].titulo, album->figDisponiveis[i].codigo);
        } // for que printa as figurinhas coladas

        // quebrar programa se o cara digitar fim
        printf("\nDigite o código da figurinha que desejas inserir\nDigite 'fim' para voltar para o menu principal ou 'todas'a para colar todas as figurinhas disponíveis : ");
        fgets(codigoDigitado, 9, stdin);
        codigoDigitado[strcspn(codigoDigitado, "\n")] = '\0';
        setbuf(stdin, NULL);

        if (strcasecmp(codigoDigitado, "fim") == 0)
        {
            break;
        } // if

        if (strcasecmp(codigoDigitado, "todas") == 0)
        {
            salvaIndiceColadas = album->qntColadas;
            album->qntColadas += album->qntDisponiveis;
            album->figColadas = (Figurinha *)realloc(album->figColadas, album->qntColadas * sizeof(Figurinha));

            for (int i = 0; i < album->qntDisponiveis; i++)
            {
                album->figColadas[salvaIndiceColadas] = album->figDisponiveis[i];
                removerFigFaltantes(album, album->figColadas[salvaIndiceColadas].codigo);
                salvaIndiceColadas++;
            }

            album->qntDisponiveis = 0;
            album->figDisponiveis = (Figurinha *)realloc(album->figDisponiveis, 1 * sizeof(Figurinha));

            green();
            printf("Todas as figurinhas disponiveis foram coladas com sucesso!\n");
            sleep(1);
            break;
        }

        for (int i = 0; i < album->qntDisponiveis; i++)
        {
            if (strcasecmp(codigoDigitado, album->figDisponiveis[i].codigo) == 0)
            // aqui nesse if de cima ta dando erro
            {
                album->qntColadas++;

                if (album->qntColadas != 1)
                {
                    album->figColadas = (Figurinha *)realloc(album->figColadas, album->qntColadas * sizeof(Figurinha));
                } /// if-else

                album->figColadas[album->qntColadas - 1] = album->figDisponiveis[i];
                salvaIndice = i;
            }
            else
            {
                contador++;
            } // se a figurinha estiver disponivel, ele cola

        } // for da linha 136

        if (contador == album->qntDisponiveis)
        {
            Bred();
            printf("\nERRO: a figurinha selecionada não está disponível para ser colada\n");
            sleep(2);
        } // if-else

        if (salvaIndice != -1)
        {
            for (int i = salvaIndice; i < album->qntDisponiveis; i++)
            {
                album->figDisponiveis[i] = album->figDisponiveis[i + 1];
            } // for que remove a figurinha escolhida da struct de figurinhas disponíveis

            album->qntDisponiveis--;
            album->figDisponiveis = (Figurinha *)realloc(album->figDisponiveis, album->qntDisponiveis * sizeof(Figurinha));
        } // if-else da linha 136

        removerFigFaltantes(album, codigoDigitado);
        system("clear");
    } /// while

} // Inserir

void funcaoListar(Album *album)
{
    printf("Você tem %i\\678 figurinhas coladas\n", album->qntColadas);
    printf("FIGURINHAS COLADAS:\n");

    for (int i = 0; i < album->qntColadas; i++)
    {
        printf("|%s|-|%s|\n", album->figColadas[i].titulo, album->figColadas[i].codigo);
    } // for que printa as figurinhas coladas

    printf("\nFIGURINHAS DISPONIVEIS:\n");

    for (int i = 0; i < album->qntDisponiveis; i++)
    {
        printf("|%s|-|%s|\n", album->figDisponiveis[i].titulo, album->figDisponiveis[i].codigo);
    } // for que printa as figurinhas coladas

    printf("\nFIGURINHAS REPETIDAS:\n");
    for (int i = 0; i < album->qntRepetidas; i++)
    {
        printf("|%s|-|%s|\n", album->figRepetidas[i].titulo, album->figRepetidas[i].codigo);
    } // for que printa as figurinhas coladas

    printf("\nFIGURINHAS FALTANTES:\n");
    for (int i = 0; i < album->qntFaltantes; i++)
    {
        printf("|%s|-|%s|\n", album->figFaltantes[i].titulo, album->figFaltantes[i].codigo);
    } // for que printa as figurinhas coladas

} // Listar

void adicionaFigFaltantes(Album *album, int indiceColadas, int indiceFaltantes)
{

    for (int i = album->qntFaltantes; i >= indiceFaltantes; i--)
    {
        album->figFaltantes[i] = album->figFaltantes[i - 1];
    } // for que adiciona a figurinha removida  no vetor de figurinhas faltantes

    album->figFaltantes[indiceFaltantes] = album->figColadas[indiceColadas];

} // função que adiciona as figurinhas removidas ao vetor de faltantes

void funcaoExcluir(Album *album)
{
    int salvaIndice = -1;
    int salvaIndiceFaltantes;
    char codigoDigitado[7];

    while (1)
    {
        Bwhite();
        salvaIndice = -1;

        if (album->qntColadas == 0)
        {
            Bred();
            printf("ERRO: você não possui nenhuma figurinha para ser excluida\n");
            sleep(1);
            break;
        } // if

        printf("FIGURINHAS COLADAS :\n");
        for (int i = 0; i < album->qntColadas; i++)
        {
            printf("|%s|-|%s|\n", album->figColadas[i].titulo, album->figColadas[i].codigo);
        } // for que printa as figurinhas coladas

        // quebrar programa se o cara digitar fim
        printf("\nDigite o código da figurinha que desejas excluir\nDigite 'fim' para voltar para o menu principal ou 'todas'a para excluir todas as figurinhas disponíveis : ");
        fgets(codigoDigitado, 7, stdin);
        codigoDigitado[strcspn(codigoDigitado, "\n")] = '\0';
        setbuf(stdin, NULL);

        if (strcasecmp(codigoDigitado, "fim") == 0)
        {
            break;
        }

        if (strcasecmp(codigoDigitado, "todas") == 0)
        {
            album->qntFaltantes += album->qntColadas;

            album->figFaltantes = (Figurinha *)realloc(album->figFaltantes, (album->qntFaltantes + 1) * sizeof(Figurinha));

            for (int i = 0; i < album->qntColadas; i++)
            {
                adicionaFigFaltantes(album, i, album->figColadas[i].indice - 1);
            }

            album->qntColadas = 0;
            album->figColadas = (Figurinha *)realloc(album->figColadas, 1 * sizeof(Figurinha));

            green();
            printf("Todas as figurinhas coladas foram removidas com sucesso!\n");
            sleep(1.5);
            break;
        }
        else
        {

            for (int i = 0; i < album->qntColadas; i++)
            {
                if (strcasecmp(album->figColadas[i].codigo, codigoDigitado) == 0)
                {
                    salvaIndice = i;
                    salvaIndiceFaltantes = (album->figColadas[i].indice - 1);
                    // nao da pra deixar null no indice pq ele nao eh um pointer
                } // if-else que remove a figurinha
            }     // for que passa pelas figurinhas ate encontrar a desejada e remove ela

            if (salvaIndice != -1)
            {
                album->qntFaltantes++;
                album->figFaltantes = (Figurinha *)realloc(album->figFaltantes, (album->qntFaltantes + 1) * sizeof(Figurinha));

                adicionaFigFaltantes(album, salvaIndice, salvaIndiceFaltantes);

                for (int i = salvaIndice; i < album->qntColadas; i++)
                {
                    album->figColadas[i] = album->figColadas[i + 1];
                } // for que passa as figurinhas uma casa p/ trás, começando a partir do índice da figurinha removida

                album->qntColadas--;
                album->figColadas = (Figurinha *)realloc(album->figColadas, album->qntColadas * sizeof(Figurinha));
            }
            else
            {
                Bred();
                printf("ERRO: a figurinha digitada não está colada\n");
                sleep(1.2);
            } // if-else
        }     // if-else (se a opção digitada não for 'todas', executa o else)
        system("clear");
    } // while

} // Excluir
//###########################---Murilo---###################################

void gerarIndiceAleatorio(Figurinha Aleatorias[])
{
    srand(time(NULL));
    // gerando indice figurinhas aleatorias

    for (int i = 0; i < 5; i++)
    {
        // Na primeira consideraremos um album com apenas 10 figurinhas ficticias
        Aleatorias[i].indice = rand() % 679 + 1;
    } // for

} // gerarIndiceAleatorio

void salvarFigAleatorias(Figurinha Aleatorias[])
{
    int qntFigAlbum = 678;
    FILE *arqFigTotais = fopen("figTotais.csv", "r");
    Figurinha figLida;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < qntFigAlbum; j++)
        {
            fscanf(arqFigTotais, "%[^,],%[^,],%[^,],%[^\n]\n", figLida.codigo, figLida.titulo, figLida.secao, figLida.tipo);
            figLida.indice = j + 1;
            if (Aleatorias[i].indice == figLida.indice)
            {
                Aleatorias[i] = figLida;
            } // if
        }     // for interno
        fseek(arqFigTotais, sizeof(char) * 22, SEEK_SET);
    } // for externo

} // salvarFigAleatórias

void printarCodigoFigurinhasAleatorias(Figurinha Aleatorias[])
{
    printf("\n-------------------------------------------------------------------------\n");
    printf("\nCodigo das figurinhas aleatorias :\n\n");

    for (int i = 0; i < 5; i++)
    {
        printf("%s - %s\n", Aleatorias[i].titulo, Aleatorias[i].codigo);
    } // for

    printf("\n-------------------------------------------------------------------------\n");

} // printarCodigoFigurinhasAleatorias

void relacaoDisponiveisRepetidas(Album *album, Figurinha Aleatorias[])
{

    int verificarColagem;
    int verificarDisponibilidade;

    // guardar todas as figurinhas em disponiveis e em repetidas
    // percorrer todas as figurinhas
    for (int i = 0; i < 5; i++)
    {
        // resetar a variavel que verifica se a figurinha ja esta salva nas fig coladas
        verificarColagem = NAO;
        // percorrer figurinhas coladas
        for (int j = 0; j < album->qntColadas; j++)
        {
            // verificar se a fig aleatoria ja esta colada
            if (strcasecmp(Aleatorias[i].codigo, album->figColadas[j].codigo) == 0)
            {
                verificarColagem = SIM;
                // salvar a aleatoria em fig repetidas
                album->qntRepetidas++;
                album->figRepetidas = (Figurinha *)realloc(album->figRepetidas, album->qntRepetidas * sizeof(Figurinha));
                album->figRepetidas[album->qntRepetidas - 1] = Aleatorias[i];
                break;
            } // if
        }     // for interno - colagem
        if (verificarColagem == NAO)
        {
            // resetar a variavel que verifica se a figurinha ja esta salva nas fig disponiveis
            verificarDisponibilidade = NAO;
            // percorrer figurinhas disponiveis
            for (int k = 0; k < album->qntDisponiveis; k++)
            {
                // verificar se a fig aleatoria ja esta disponivel
                if (strcasecmp(Aleatorias[i].codigo, album->figDisponiveis[k].codigo) == 0)
                {
                    verificarDisponibilidade = SIM;
                    // salvar a aleatoria em fig repetidas
                    album->qntRepetidas++;
                    album->figRepetidas = (Figurinha *)realloc(album->figRepetidas, album->qntRepetidas * sizeof(Figurinha));
                    album->figRepetidas[album->qntRepetidas - 1] = Aleatorias[i];
                    break;
                } // if
            }     // for - disponivel
            if (verificarDisponibilidade == NAO)
            {
                album->qntDisponiveis++;
                album->figDisponiveis = (Figurinha *)realloc(album->figDisponiveis, album->qntDisponiveis * sizeof(Figurinha));
                album->figDisponiveis[album->qntDisponiveis - 1] = Aleatorias[i];
            } // if - disponibilidade
        }     // if - nao colada
    }         // for externo
} // verificarColagem

void comprarFigurinha(Album *album, Figurinha Aleatorias[])
{
    float valor_pacotinho = 5;
    int numero_pacotinhos = 0;
    int numero_max_pacotinhos;

    if (album->dinheiro_atual < valor_pacotinho)
    {
        printf("\nVoce nao tem dinheiro o suficiente para comprar um pacotinho\n");
    }
    else
    {
        numero_max_pacotinhos = album->dinheiro_atual / valor_pacotinho;
        printf("Digite quantos pacotinhos voce quer (OBS : voce pode comprar no maximo %d pacotinhos) : ", numero_max_pacotinhos);
        scanf("%d", &numero_pacotinhos);
        if (numero_pacotinhos > numero_max_pacotinhos)
        {
            printf("\nVoce nao tem dinheiro para comprar %d pacotinhos\n", numero_pacotinhos);
        }
        else
        {
            // arrancar dinheiro do menino e gerar pacotinho de fig
            album->dinheiro_atual -= valor_pacotinho * numero_pacotinhos;
            album->gastoPacote += valor_pacotinho * numero_pacotinhos;

            // gerar os pacotinhos comprados
            for (int i = 0; i < numero_pacotinhos; i++)
            {
                printf("\n--------Pacotinho %d--------\n", i + 1);
                gerarIndiceAleatorio(Aleatorias);
                salvarFigAleatorias(Aleatorias);
                printarCodigoFigurinhasAleatorias(Aleatorias);
                relacaoDisponiveisRepetidas(album, Aleatorias);
                sleep(1.3);
            } // for
            printf("\nVoce tem %.2f dinheiros e ja gastou %.2f com pacotinhos\n", album->dinheiro_atual, album->gastoPacote);
        } // else
    }     // else
} // fim funcao

void folhearAlbum(Album *album)
{
    int contadorSecao = 0;
    FILE *arqFigTotais = fopen("figTotais.csv", "r");
    char guardarSecao[2][29];
    Figurinha figLida;
    char todasSecoesAlbum[37][29];
    int folhear;
    int secaoAtual = 0;

    // preencher o vetor de secoes do album
    strcpy(todasSecoesAlbum[0], "Content");
    strcpy(todasSecoesAlbum[1], "Stadiums");
    strcpy(todasSecoesAlbum[35], "FIFA Museum");
    strcpy(todasSecoesAlbum[36], "Coca-Cola Team Believers");

    fseek(arqFigTotais, sizeof(char) * 22, SEEK_SET);
    for (int i = 0; i < 338; i++)
    {
        fscanf(arqFigTotais, "%[^,],%[^,],%[^,],%[^\n]\n", figLida.codigo, figLida.titulo, figLida.secao, figLida.tipo);
        strcpy(guardarSecao[0], figLida.secao);

        fscanf(arqFigTotais, "%[^,],%[^,],%[^,],%[^\n]\n", figLida.codigo, figLida.titulo, figLida.secao, figLida.tipo);
        strcpy(guardarSecao[1], figLida.secao);

        printf("%s - %s\n", guardarSecao[0], guardarSecao[1]);
        if (strcasecmp(guardarSecao[0], guardarSecao[1]) != 0)
        {
            strcpy(todasSecoesAlbum[contadorSecao + 2], guardarSecao[0]);
            contadorSecao++;
        } // if
    }     // for

    printf("BEM VINDO(A) AO SEU ÁLBUM DA COPA\nDigite 1 para voltar à página anterior, 2 para ir à próxima página ou 0 para sair do álbum\n\n\n\n");

    do
    {
        int contafig = 0;
        Bwhite();
        printf("          ----------------------------------------------------  %s  -------------------------------------------------\n\n\n", todasSecoesAlbum[secaoAtual]);

        for (int i = 0; i < album->qntColadas; i++)
        {
            if (strcasecmp(album->figColadas[i].secao, todasSecoesAlbum[secaoAtual]) == 0)
            {
                green();
                printf("          |%-35s - %-7s|           ", album->figColadas[i].titulo, album->figColadas[i].codigo);
                contafig++;
                if (contafig % 2 == 0 && contafig != 0)
                {
                    printf("\n");
                }
            }
        }

        for (int i = 0; i < album->qntDisponiveis; i++)
        {
            if (strcasecmp(album->figDisponiveis[i].secao, todasSecoesAlbum[secaoAtual]) == 0)
            {
                yellow();
                printf("          |%-35s - %-7s|           ", album->figDisponiveis[i].titulo, album->figDisponiveis[i].codigo);
                contafig++;

                if (contafig % 2 == 0 && contafig != 0)
                {
                    printf("\n");
                }
            }
        }

        for (int i = 0; i < album->qntFaltantes; i++)
        {
            if (strcasecmp(album->figFaltantes[i].secao, todasSecoesAlbum[secaoAtual]) == 0)
            {
                red();
                printf("          |%-35s - %-7s|           ", album->figFaltantes[i].titulo, album->figFaltantes[i].codigo);
                contafig++;

                if (contafig % 2 == 0 && contafig != 0)
                {
                    printf("\n");
                }
            }
        }

        Bwhite();
        printf("\n\n\n0: fechar album\n1: anterior\n2: seguinte\n");

        do
        {
            scanf("%d", &folhear);
        } while (folhear != 1 && folhear != 2 && folhear != 0);

        if (folhear == 0)
        {
            Bgreen();
            printf("\nFechando album...\n");
            break;
        }

        if (folhear == 1 && secaoAtual == 0)
        {
            secaoAtual = 36;
        }
        else if (folhear == 2 && secaoAtual == 36)
        {
            secaoAtual = 0;
        }
        else if (folhear == 1)
        {
            secaoAtual--;
        }
        else
        {
            secaoAtual++;
        }

        system("clear");
    } while (1);

    fclose(arqFigTotais);

} // fim funcao

//---------------------INTERFACE DE ABERTURA DO ALBUM

void quiz(Album *album)
{
    FILE *arqFigTotais = fopen("figTotais.csv", "r");
    Figurinha figAleatoria;
    char selecaoDigitada[29];
    int indiceAleatorio;

    do
    {

        srand(time(NULL));

        indiceAleatorio = rand() % 679 + 1;
        sleep(0.1);

        printf("Bem-vindo(a) ao Quiz Álbum da Copa 2022!\nCada acerto lhe renderá R$5,00\nDigite 'fim' para sair do quiz\n\n");

        do
        {
            indiceAleatorio = rand() % 679 + 1;
            // limita o intervalo do indice aleatorio para 20>indice<670
            do
            {
                indiceAleatorio = rand() % 679 + 1;
            } while (indiceAleatorio > 0 && indiceAleatorio < 20 || indiceAleatorio > 669);

            fseek(arqFigTotais, sizeof(char) * 22, SEEK_SET);
            for (int j = 0; j < 678; j++)
            {
                fscanf(arqFigTotais, "%[^,],%[^,],%[^,],%[^\n]\n", figAleatoria.codigo, figAleatoria.titulo, figAleatoria.secao, figAleatoria.tipo);
                figAleatoria.indice = j + 1;
                if (figAleatoria.indice == indiceAleatorio)
                {
                    break;
                } // if
            }     // for interno
        } while (strcasecmp(figAleatoria.titulo, "Team Shot") == 0 || strcasecmp(figAleatoria.titulo, "Team Logo") == 0);

        printf("O jogador %s pertence à qual seleção?\n", figAleatoria.titulo);
        fgets(selecaoDigitada, 29, stdin);
        selecaoDigitada[strcspn(selecaoDigitada, "\n")] = '\0';
        setbuf(stdin, NULL);

        // retorna ao menu principal se o usuário digitar "fim"
        if (strcasecmp(selecaoDigitada, "fim") == 0)
        {
            cyan();
            printf("Obrigado por jogar!\n");
            sleep(1);
            break;
        }

        // vê se o usuário acertou a pergunta
        if (strcasecmp(selecaoDigitada, figAleatoria.secao) == 0)
        {
            Bgreen();
            printf("Parabéns, você acertou!\n");
            album->dinheiro_atual += 5;
            sleep(1);
            system("clear");
        }
        else
        {
            Bred();
            printf("Você errou :(\nTente novamente\n");
            sleep(1);
            system("clear");
        } // if--else

    } while (1);
    fclose(arqFigTotais);

} // fim da função quiz

//---------------------MENU INICIAL
void menu(Album *album, Figurinha Aleatorias[])
{
    // preciso também da relação de figurinhas disponíveis, por baseado nela que faço toda a alteração
    int opt0, ctrl = 1;

    while (ctrl == 1)
    {
        system("clear");
        Bwhite();
        printf("%s seja bem vindo ao album da copa virtual!\nO que desejas fazer?\n\n", album->nome);
        white();
        printf("\nBem-vindo(a) %s\nVocê tem RS%.2f\n\n", album->nome, album->dinheiro_atual);
        printf("1 - Colar uma figurinha\n2 - Listar todas as figurinhas\n3 - Remover uma figurinha do album\n4 - Comprar pacotinho de figurinha\n5 - Quiz Àlbum da Copa 2022\n6 - Abrir album\n7 - Salvar o programa\n8 - Encerrar e salvar programa\n");

        do
        {
            scanf("%d", &opt0);
            setbuf(stdin, NULL);
        } while (opt0 < 1 || opt0 > 8);

        switch (opt0)
        {
        case 1:
            system("clear");
            funcaoInserir(album);
            sleep(1);
            break;

        case 2:
            system("clear");
            funcaoListar(album);
            sleep(10);
            break;

        case 3:
            system("clear");
            funcaoExcluir(album);
            sleep(1);
            break;

        case 4:
            system("clear");
            comprarFigurinha(album, Aleatorias);
            setbuf(stdin, NULL);
            sleep(3);
            system("clear");
            funcaoInserir(album);
            break;

        case 5:
            system("clear");
            quiz(album);
            sleep(1);
            break;

        case 6:
            system("clear");
            folhearAlbum(album);
            sleep(1);
            break;

        case 7:
            system("clear");
            salvarFigurinhasAlbum(album);
            Bred();
            printf("Suas figurinhas foram salvas com sucesso :)\n");
            sleep(2);
            break;

        case 8:
            salvarFigurinhasAlbum(album);
            free(album->figColadas);
            free(album->figFaltantes);
            free(album->figRepetidas);
            free(album->figDisponiveis);
            system("clear");
            ctrl = 2;
            Bred();
            printf("Suas figurinhas foram salvas com sucesso :)\n");
            printf("Programa encerrado\n");
            sleep(1);
            break;
        } // switch
        setbuf(stdin, NULL);
    } // while
} // menu

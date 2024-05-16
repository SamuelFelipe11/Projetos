import csv
class Processo:
    def __init__(self, processo, tempo_chegada, tempo_CPU, tempo_execucao, tempo_resposta, tempo_espera, finalizado):
        self.processo = processo
        self.tempo_chegada = tempo_chegada
        self.tempo_CPU = tempo_CPU
        self.tempo_execucao = tempo_execucao
        self.tempo_resposta = tempo_resposta
        self.tempo_espera = tempo_espera
        self.finalizado = finalizado


def ler_arquivo(nome_arquivo):
    dados = []
    with open(nome_arquivo, "r") as arquivo:
        leitor_csv = csv.reader(arquivo)
        for linha in leitor_csv:
            dados.append(linha)

    return dados

def monta_lista_processos(nome_arquivo):
    dados = ler_arquivo(nome_arquivo)
    lista_processos = []
    processos = dados[0]
    tempo_CPU = dados[1]
    tempo_chegada = dados[2]

    for i in range (0, len(processos)):
        lista_processos.append(Processo(processos[i], int(tempo_chegada[i]), int(tempo_CPU[i]), int(tempo_CPU[i]), 0, 0, False))

    lista_processos.sort(key = lambda processo: processo.tempo_chegada)

    return lista_processos

def exibe_fila_processos(fila_processos):
    print("|0|", end="")
    for item in fila_processos:
        print("---" + item[0] + "---|" + str(item[1]) + "|", end="")


def round_robin():
    lista_processos = monta_lista_processos("Dados.csv")
    tempo_atual = 0
    fila_processos = []
    processo_anterior = []

    while True:
        tempo_execucao = 0
        for processo in lista_processos[0:len(lista_processos)]:
            if processo.tempo_chegada <= tempo_atual and processo.finalizado == False:
                if processo.tempo_execucao <= 5:
                    processo.finalizado = True
                    tempo_atual += processo.tempo_execucao
                    processo.tempo_execucao = 0
                    processo.tempo_resposta = tempo_atual-processo.tempo_chegada
                    processo.tempo_espera = processo.tempo_resposta - processo.tempo_CPU
                else:
                    tempo_atual += 5
                    processo.tempo_execucao -=5
                    processo_anterior.append(processo)

                fila_processos.append((processo.processo, tempo_atual))

            if len(processo_anterior) != 0 and processo_anterior[0] != processo:                
                for processo in processo_anterior:
                    if processo.tempo_execucao <= 5:
                        processo.finalizado = True
                        tempo_atual += processo.tempo_execucao
                        processo.tempo_execucao = 0
                        processo.tempo_resposta = tempo_atual-processo.tempo_chegada
                        processo.tempo_espera = processo.tempo_resposta - processo.tempo_CPU
                    else:
                        tempo_atual += 5
                        processo.tempo_execucao -= 5

                    fila_processos.append((processo.processo, tempo_atual))
                    processo_anterior.remove(processo)
            tempo_execucao += processo.tempo_execucao

        if tempo_execucao == 0: break
    
    relatorio(lista_processos, fila_processos, "Round-Robin")


def relatorio(lista_processos, fila_processos, nome_politica):
    print(f"Processos na Fila do {nome_politica}:")
    for item in fila_processos:
        print(item[0], end = " ")

    print("\n\n\nTempo de CPU requerida pelos processos:")
    for item in lista_processos:
        print(item.tempo_CPU, end = " ")

    print("\n\n\nTempo de Chegada dos processos:")
    for item in lista_processos:
        print(item.tempo_chegada, end = " ")

    print("\n\n\nLINHA DO TEMPO")
    exibe_fila_processos(fila_processos)

    print("\n\n\nTempo de Espera de cada processo:")
    tme = 0
    for item in lista_processos:
        print(item.processo, end = " ")
    print("")
    for item in lista_processos:
        print(item.tempo_espera, end = " ")
        tme += item.tempo_espera


    print("\n\n\nTempo de Resposta de cada processo:")
    tmr = 0
    for item in lista_processos:
        print(item.processo, end = " ")
    print("")
    for item in lista_processos:
        print(item.tempo_resposta, end = " ")
        tmr += item.tempo_resposta

    print("\n\n\nTempo Médio de Resposta: ", tmr/len(lista_processos))
    print("\nTempo Médio de Espera: ", tme/len(lista_processos))

round_robin()
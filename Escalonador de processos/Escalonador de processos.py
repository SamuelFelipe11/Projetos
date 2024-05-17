import csv

class Processo:
    def __init__(self, processo, tempo_CPU, priority, tempo_chegada, tempo_execucao, tempo_resposta, tempo_espera, finalizado):
        self.processo = processo
        self.tempo_CPU = tempo_CPU
        self.priority = priority
        self.tempo_chegada = tempo_chegada
        self.tempo_execucao = tempo_execucao
        self.tempo_resposta = tempo_resposta
        self.tempo_espera = tempo_espera
        self.finalizado = finalizado

class ProcessScheduleSimulator:
    def ler_arquivo(self, nome_arquivo):
        dados = []
        with open(nome_arquivo, "r") as arquivo:
            leitor_csv = csv.reader(arquivo)
            for linha in leitor_csv:
                dados.append(linha)
        return dados

    def monta_lista_processos(self, nome_arquivo, has_priority = False):
        dados = self.ler_arquivo(nome_arquivo)
        lista_processos = []
        processos = dados[0]

        if has_priority:
            tempo_CPU = dados[1]
            priority = dados[2]
            tempo_chegada = dados[3]
        else:
            tempo_CPU = dados[1]
            tempo_chegada = dados[2]


        if has_priority:
            for i in range(0, len(processos)):
                lista_processos.append(Processo(processos[i],  int(tempo_CPU[i]), int(priority[i]), int(tempo_chegada[i]), int(tempo_CPU[i]), None, None, False))
        else:
            for i in range(0, len(processos)):
                lista_processos.append(Processo(processos[i],  int(tempo_CPU[i]), None, int(tempo_chegada[i]), int(tempo_CPU[i]), None, None, False))

        
        lista_processos.sort(key=lambda processo: processo.tempo_chegada)

        return lista_processos
    
    def TMR(self, process, conclusion_time):
        process.tempo_resposta = conclusion_time - process.tempo_chegada

    def TME(self, process):
        process.tempo_espera = process.tempo_resposta - process.tempo_CPU

    def total_exec_time(self, process_list):
        time = 0
        for item in process_list:
            time += item.tempo_execucao
        
        return time

    def exibe_fila_processos(self, fila_processos):
        print("|0|", end="")
        for item in fila_processos:
            print("---" + item[0] + "---|" + str(item[1]) + "|", end="")

    def iterate_list(self, process_list, field):
        for item in process_list:
            print(getattr(item,field), end = " ")
            
    def relatorio(self, lista_processos, fila_processos, nome_politica):
        print(f"Processos na Fila do {nome_politica}:")
        for item in fila_processos:
            print(item[0], end=" ")

        print("\n\n\nTempo de CPU requerida pelos processos:")
        self.iterate_list(lista_processos, "tempo_CPU")

        print("\n\n\nTempo de Chegada dos processos:")
        self.iterate_list(lista_processos, "tempo_chegada")

        print("\n\n\nLINHA DO TEMPO")
        self.exibe_fila_processos(fila_processos)

        print("\n\n\nTempo de Espera de cada processo:")
        tme = 0
        self.iterate_list(lista_processos, "processo")
        print("")
        for item in lista_processos:
            print(item.tempo_espera, end=" ")
            tme += item.tempo_espera

        print("\n\n\nTempo de Resposta de cada processo:")
        tmr = 0
        self.iterate_list(lista_processos, "processo")
        print("")
        for item in lista_processos:
            print(item.tempo_resposta, end=" ")
            tmr += item.tempo_resposta

        print("\n\n\nTempo Médio de Resposta: ", tmr / len(lista_processos))
        print("\nTempo Médio de Espera: ", tme / len(lista_processos))

class RoundRobin(ProcessScheduleSimulator):
    def simulate(self, nome_arquivo):
        process_list = self.monta_lista_processos(nome_arquivo)
        process_queue = []
        current_time = 0
        while self.total_exec_time(process_list):


        print(process_queue)       
        #self.relatorio(process_list, process_queue, "Round-Robin")

class SJF(ProcessScheduleSimulator):
    def simulate(self, archive_name):
        process_list = self.monta_lista_processos(archive_name)
        current_time = 0
        process_queue = []
        
        
        while self.total_exec_time(process_list):
            shortest = float('inf')
            queue =  None
            for item in process_list:
                if item.tempo_chegada <= current_time and item.tempo_execucao < shortest and item.finalizado == False:
                    queue = item
                    shortest = item.tempo_execucao                    

            current_time += 1
            
            if queue is not None: 
                queue.tempo_execucao -=1
                if not process_queue or queue.processo != process_queue[-1][0]:
                    process_queue.append((queue.processo, current_time))
                else:
                    process_queue[-1] = (queue.processo, current_time)

            self.TMR(queue, current_time)
            self.TME(queue)

            if queue.tempo_execucao == 0:
                queue.finalizado = True
            
        
        self.relatorio(process_list, process_queue, "SJF")
        
class StaticPriority(ProcessScheduleSimulator):
    def simulate(self, archive_name):
        process_list = self.monta_lista_processos(archive_name, True)
        current_time = 0
        process_queue = []
        
        
        while self.total_exec_time(process_list):
            shortest = float('inf')
            queue =  None
            for item in process_list:
                if item.tempo_chegada <= current_time and item.priority < shortest and item.finalizado == False:
                    queue = item
                    shortest = item.priority                   

            current_time += 1
            queue.tempo_execucao -=1
            
            if queue is not None: 
                if not process_queue or queue.processo != process_queue[-1][0]:
                    process_queue.append((queue.processo, current_time))
                else:
                    process_queue[-1] = (queue.processo, current_time)

            self.TMR(queue, current_time)
            self.TME(queue)
            
            if queue.tempo_execucao == 0:
                queue.finalizado = True
                
        self.relatorio(process_list, process_queue, "PRIORIDADE ESTATISTICA")


schedule1 = RoundRobin()
schedule2 = SJF()
schedule3 = StaticPriority()

schedule1.simulate("Dados.csv")
#schedule2.simulate("Dados.csv")
#schedule3.simulate("Dados3.csv")

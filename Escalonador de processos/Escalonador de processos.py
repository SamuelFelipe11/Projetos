import csv, sys

class Processo:
    def __init__(self, process, CPU_time, priority, arrival_time, exec_time, answer_time, waiting_time, finished):
        self.process = process
        self.CPU_time = CPU_time
        self.priority = priority
        self.arrival_time = arrival_time
        self.exec_time = exec_time
        self.answer_time = answer_time
        self.waiting_time = waiting_time
        self.finished = finished

class ProcessScheduleSimulator:
    def arq_read(self, nome_arquivo):
        data = []
        with open(nome_arquivo, "r") as arquivo:
            leitor_csv = csv.reader(arquivo)
            for linha in leitor_csv:
                data.append(linha)
        return data

    def generate_process_list(self, nome_arquivo, has_priority = False):
        data = self.arq_read(nome_arquivo)
        process_list = []
        processes = data[0]

        if has_priority:
            CPU_time = data[1]
            priority = data[2]
            arrival_time = data[3]
        else:
            CPU_time = data[1]
            arrival_time = data[2]
        
        if len(data) == 4 and sys.argv[1].lower() != "prioridade estatica":
            print("Formato do arquivo de entrada inválido para a política escolhida")
            exit(1)

        if has_priority:
            for i in range(0, len(processes)):
                process_list.append(Processo(processes[i],  int(CPU_time[i]), int(priority[i]), int(arrival_time[i]), int(CPU_time[i]), None, None, False))
        else:
            for i in range(0, len(processes)):
                process_list.append(Processo(processes[i],  int(CPU_time[i]), None, int(arrival_time[i]), int(CPU_time[i]), None, None, False))

        
        #process_list.sort(key=lambda process: process.arrival_time)

        return process_list
    
    def TMR(self, process, conclusion_time):
        process.answer_time = conclusion_time - process.arrival_time

    def TME(self, process):
        process.waiting_time = process.answer_time - process.CPU_time

    def total_exec_time(self, process_list):
        time = 0
        for item in process_list:
            time += item.exec_time
        
        return time

    def exibe_process_queue(self, process_queue):
        print("|0|", end="")
        for item in process_queue:
            print("---" + item[0] + "---|" + str(item[1]) + "|", end="")

    def iterate_list(self, process_list, field):
        for item in process_list:
            print(getattr(item,field), end = " ")
            
    def relatorio(self, process_list, process_queue, schedule_name):
        print(f"processos na Fila do {schedule_name}:")
        for item in process_queue:
            print(item[0], end=" ")

        print("\n\n\nTempo de CPU requerida pelos processos:")
        self.iterate_list(process_list, "CPU_time")

        print("\n\n\nTempo de Chegada dos processos:")
        self.iterate_list(process_list, "arrival_time")

        print("\n\n\nLINHA DO TEMPO")
        self.exibe_process_queue(process_queue)

        print("\n\n\nTempo de Espera de cada processo:")
        tme = 0
        self.iterate_list(process_list, "process")
        print("")
        for item in process_list:
            print(item.waiting_time, end=" ")
            tme += item.waiting_time

        print("\n\n\nTempo de Resposta de cada processo:")
        tmr = 0
        self.iterate_list(process_list, "process")
        print("")
        for item in process_list:
            print(item.answer_time, end=" ")
            tmr += item.answer_time

        print("\n\n\nTempo Médio de Resposta: ", tmr / len(process_list))
        print("\nTempo Médio de Espera: ", tme / len(process_list))

class RoundRobin(ProcessScheduleSimulator):
    def simulate(self, nome_arquivo):
        process_list = self.generate_process_list(nome_arquivo)
        process_queue = []
        temporary_queue = []
        current_time = 0
        previous = None

        while self.total_exec_time(process_list):
            time_adjust = 0

            for item in process_list:
                if item.arrival_time <= current_time and item.finished == False and item not in temporary_queue and item.process not in [item[0] for item in process_queue]:
                    temporary_queue.append(item)                            

            if previous:
               temporary_queue.append(previous)

            previous, time_adjust = self.queue(temporary_queue, process_queue, current_time)            

            current_time += time_adjust

        self.relatorio(process_list, process_queue, "Round-Robin")

    def queue(self, temporary_queue, process_queue, current_time):
        previous = None
        time = 0
        if temporary_queue:
            if temporary_queue[0].exec_time <= 5:
                time += temporary_queue[0].exec_time
                temporary_queue[0].finished = True
                temporary_queue[0].exec_time = 0
            else:
                time += 5
                previous = temporary_queue[0]
                temporary_queue[0].exec_time -= 5
            
            process_queue.append((temporary_queue[0].process, current_time +time))
            self.TMR(temporary_queue[0], current_time + time)
            self.TME(temporary_queue[0])            
            temporary_queue.pop(0)

        return previous, time
    
class SJF(ProcessScheduleSimulator):
    def simulate(self, archive_name):
        process_list = self.generate_process_list(archive_name)
        current_time = 0
        process_queue = []
        
        
        while self.total_exec_time(process_list):
            shortest = float('inf')
            queue =  None
            for item in process_list:
                if item.arrival_time <= current_time and item.exec_time < shortest and item.finished == False:
                    queue = item
                    shortest = item.exec_time                    

            current_time += 1
            
            if queue is not None: 
                queue.exec_time -=1
                if not process_queue or queue.process != process_queue[-1][0]:
                    process_queue.append((queue.process, current_time))
                else:
                    process_queue[-1] = (queue.process, current_time)

            self.TMR(queue, current_time)
            self.TME(queue)

            if queue.exec_time == 0:
                queue.finished = True
            
        
        self.relatorio(process_list, process_queue, "SJF")
        
class StaticPriority(ProcessScheduleSimulator):
    def simulate(self, archive_name):
        process_list = self.generate_process_list(archive_name, True)
        current_time = 0
        process_queue = []
        
        
        while self.total_exec_time(process_list):
            shortest = float('inf')
            queue =  None
            for item in process_list:
                if item.arrival_time <= current_time and item.priority < shortest and item.finished == False:
                    queue = item
                    shortest = item.priority                   

            current_time += 1
            queue.exec_time -=1
            
            if queue is not None: 
                if not process_queue or queue.process != process_queue[-1][0]:
                    process_queue.append((queue.process, current_time))
                else:
                    process_queue[-1] = (queue.process, current_time)

            self.TMR(queue, current_time)
            self.TME(queue)
            
            if queue.exec_time == 0:
                queue.finished = True
                
        self.relatorio(process_list, process_queue, "Prioridade Estática")

if __name__ == "__main__":
    
    if len(sys.argv) != 3:
        print("ERRO! QUANTIDADE DE PARÂMETROS INVÁLIDA\n")
        exit(1)


    if sys.argv[1].lower() == "round-robin":
        schedule = RoundRobin()
    elif sys.argv[1].lower() == "sjf":
        schedule = SJF()
    elif sys.argv[1].lower() == "prioridade estatica":
        schedule = StaticPriority()
    else:
        print("Política de escalonamento inválida!")
        exit(1)

    schedule.simulate(sys.argv[2])

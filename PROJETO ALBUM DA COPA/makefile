
main: Main.o funcoes.o
	@echo "Gerando arquivo executável"
	gcc *.o -o Main

Main.o: Main.c
	@echo "Compilando e gerando objetos"
	gcc -c *.c

funcoes.o: funcoes.c
	@echo "Compilando e gerando objetos"
	gcc -c *.c

clean:
	@echo "Apagando objetos e executáveis antigos"
	rm -f *.o Main

run:
	./Main

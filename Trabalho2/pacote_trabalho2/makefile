CP = gcc
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo""
	@echo "Executando..."
	@echo
	./exe

exit: imagem.o main.o
	$(CP) -o exe imagem.o main.o -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

main.o: main.c
	$(CP) -o main.o -c main.c

clean:
	rm -rf *.o
	rm -rf exe

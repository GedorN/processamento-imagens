CP = gcc
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo
	@echo "Executando..."
	./exe

exit: main.o imagem.o base.o filtros2d.o
	$(CP) -o exe imagem.o main.o filtros2d.o base.o -lm

filtros2d.o: filtros2d.c base.o imagem.o
	$(CP) -o filtros2d.o -c imagem.o base.o filtros2d.c -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

base.o: base.c 
	$(CP) -o base.o -c base.c

main.o: main.c
	$(CP) -o main.o -c main.c

clean:
	rm -rf *.o
	rm -rf exe

CP = gcc
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo
	@echo "Executing..."
	./exe

exit: main.o imagem.o base.o filtros2d.o segmenta.o geometria.o cores.o
	$(CP) -o exe main.o imagem.o base.o filtros2d.o segmenta.o geometria.o cores.o -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

base.o: base.c
	$(CP) -o base.o -c base.c

filtros2d.o: filtros2d.c
	$(CP) -o filtros2d.o -c filtros2d.c

segmenta.o: segmenta.c
	$(CP) -o segmenta.o -c segmenta.c

geometria.o: geometria.c
	$(CP) -o geometria.o -c geometria.c

cores.o: cores.c
	$(CP) -o cores.o -c cores.c

main.o: main.c
	$(CP) -o main.o -c main.c
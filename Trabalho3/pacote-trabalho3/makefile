CP = gcc
EXEC = exe
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo 
	@echo "Executando arquivo"
	@echo
	./exe

exit: filtros2d.o base.o
	$(CP) -o $(EXEC) imagem.o base.o filtros2d.o main.c -lm

filtros2d.o: filtros2d.c base.o imagem.o
	$(CP) -o filtros2d.o -c imagem.o base.o filtros2d.c -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

base.o: base.c 
	$(CP) -o base.o -c base.c

clean:
	rm -rf *.o
	rm -rf $(EXEC)

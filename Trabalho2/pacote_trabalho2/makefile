CP = gcc
EXEC = prog
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo "Arquivo \"$(EXEC)\" gerado..."
	@echo

exit: imagem.o main.o filtros.o
	$(CP) -o $(EXEC) imagem.o filtros.o main.o -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

filtros.o: filtros.c
	$(CP) -o filtros.o -c filtros.c -lm

main.o: main.c
	$(CP) -o main.o -c main.c

clean:
	rm -rf *.o
	rm -rf $(EXEC)

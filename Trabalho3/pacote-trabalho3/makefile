CP = gcc
all: gaussian boxblur
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo

boxblur: filtros2d.o base.o
	$(CP) -o boxblur imagem.o base.o filtros2d.o main.c -lm -DBOX_BLUR 

gaussian: filtros2d.o base.o
	$(CP) -o gaussian imagem.o base.o filtros2d.o main.c -lm -DGAUSSIAN 

filtros2d.o: filtros2d.c base.o imagem.o
	$(CP) -o filtros2d.o -c imagem.o base.o filtros2d.c -lm

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

base.o: base.c 
	$(CP) -o base.o -c base.c

clean:
	rm -rf *.o
	rm -rf $(EXEC)

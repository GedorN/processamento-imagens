CP = gcc
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo
	@echo "Executing..."
	./exe

exit: main.o
	$(CP) -o exe main.o -lm

main.o: main.c
	$(CP) -o main.o -c main.c
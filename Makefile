CFLAGS = -g -ansi -Wall -pedantic -o $@
EXE_DEPS = macro_list.o macro_dict.o pre_assembler.o assembler.o errors.o symbol_list.o symbol_dict.o

assembler: $(EXE_DEPS)
	gcc -g $^ $(CFLAGS)

%.o: %.c %.h
	gcc -g -c $< $(CFLAGS)

clean:
	rm -rf *.o



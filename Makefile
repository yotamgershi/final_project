CFLAGS = -g -ansi -Wall -pedantic -o $@
EXE_DEPS = mcro_dict.o pre_assembler.o symbol_dict.o first_pass.o second_pass.o assembler.o

assembler: $(EXE_DEPS)
	gcc -g $^ $(CFLAGS) 

%.o: %.c %.h
	gcc -g -c $< $(CFLAGS)

clean:
	rm -rf *.o
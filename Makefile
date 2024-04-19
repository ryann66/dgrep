CC=g++
CFLAGS=--std=c++17
DFLAGS=-g -Og
BFLAGS=-O2
LFLAGS=
LIBS=
OBJS=Ast.o main.o Metastring.o Parser.o Tokenizer.o Tokens.o errors.o NFA.o Alphabet.o NFAevaluator.o

# setup
debug: override CFLAGS+=$(DFLAGS)
debug: all

build: override CFLAGS+=$(BFLAGS)
build: all

all: dgrep

dgrep: $(OBJS)
	$(CC) $(LFLAGS) -o dgrep $(OBJS) $(LIBS)

# objects
Alphabet.o: Alphabet.cc Alphabet.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Ast.o: Ast.cc AST.h Metastring.h Tokens.h Alphabet.h NFAcomponents.h errors.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

NFA.o: NFA.cc NFA.h AST.h Metastring.h Tokens.h Alphabet.h NFAcomponents.h errors.h globals.h NFAevaluator.h
	$(CC) $(CFLAGS) -c -o $@ $<

NFAevaluator.o: NFAevaluator.cc NFAevaluator.h NFA.h AST.h Metastring.h Tokens.h Alphabet.h NFAcomponents.h errors.h
	$(CC) $(CFLAGS) -pthread -c -o $@ $<

main.o: main.cc Tokens.h Alphabet.h Tokenizer.h Parser.h AST.h Metastring.h NFAcomponents.h errors.h globals.h NFA.h
	$(CC) $(CFLAGS) -c -o $@ $<

Metastring.o: Metastring.cc Metastring.h globals.h Alphabet.h errors.h
	$(CC) $(CFLAGS) -c -o $@ $<

Parser.o: Parser.cc Parser.h AST.h Metastring.h Tokens.h Alphabet.h NFAcomponents.h errors.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokenizer.o: Tokenizer.cc Tokenizer.h Tokens.h Alphabet.h errors.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokens.o: Tokens.cc Tokens.h Alphabet.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

errors.o: errors.cc errors.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	/bin/rm -f *.o dgrep
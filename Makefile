CC=g++
CFLAGS=--std=c++17
DFLAGS=-g -Og
BFLAGS=-O2
LFLAGS=
LIBS=
OBJS=Ast.o main.o Metastring.o Parser.o Tokenizer.o Tokens.o truncation_error.o NFA.o Alphabet.o NFAevaluator.o

# setup
debug: override CFLAGS+=$(DFLAGS)
debug: all

build: override CFLAGS+=$(BFLAGS)
build: all

all: dgrep

dgrep: $(OBJS)
	$(CC) $(LFLAGS) -o dgrep $(OBJS) $(LIBS)

# header structure
Ast.h: Metastring.h Tokens.h
Parser.h: Ast.h Tokens.h
Tokenizer.h: Tokens.h

# cc header inclusions
Ast.cc: truncation_error.h
Metastring.cc: globals.h truncation_error.h
Parser.cc: syntax_error.h
Tokenizer.cc: syntax_error.h
Tokens.cc: globals.h

# objects
Ast.o: Ast.cc Ast.h Metastring.h Tokens.h truncation_error.h globals.h NFAcomponents.h
	$(CC) $(CFLAGS) -c -o $@ $<

NFA.o: NFA.cc NFA.h NFAcomponents.h
	$(CC) $(CFLAGS) -c -o $@ $<

NFAevaluator.o: NFAevaluator.cc NFAevaluator.h
	$(CC) $(CFLAGS) -pthread -c -o $@ $<

main.o: main.cc Tokens.h Tokenizer.h Parser.h Ast.h Metastring.h truncation_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Metastring.o: Metastring.cc Metastring.h globals.h truncation_error.h
	$(CC) $(CFLAGS) -c -o $@ $<

Parser.o: Parser.cc Parser.h Ast.h Metastring.h Tokens.h syntax_error.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokenizer.o: Tokenizer.cc Tokenizer.h Tokens.h syntax_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokens.o: Tokens.cc Tokens.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

truncation_error.o: truncation_error.cc truncation_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Alphabet.o: Alphabet.cc Alphabet.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	/bin/rm -f *.o dgrep
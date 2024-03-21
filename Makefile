CC=g++
CFLAGS=
DFLAGS=-g -Og
BFLAGS=-O2
LFLAGS=
LIBS=
OBJS=Ast.o LinkedString.o main.o Metastring.o Parser.o Tokenizer.o Tokens.o truncation_error.o

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
Ast.o: Ast.cc Ast.h Metastring.h Tokens.h truncation_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

LinkedString.o: LinkedString.cc LinkedString.h LinkedStringPrivate.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.cc Tokens.h Tokenizer.h Parser.h Ast.h Metastring.h truncation_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Metastring.o: Metastring.cc Metastring.h globals.h truncation_error.h LinkedString.h LinkedStringPrivate.h
	$(CC) $(CFLAGS) -c -o $@ $<

Parser.o: Parser.cc Parser.h Ast.h Metastring.h Tokens.h syntax_error.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokenizer.o: Tokenizer.cc Tokenizer.h Tokens.h syntax_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tokens.o: Tokens.cc Tokens.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

truncation_error.o: truncation_error.cc truncation_error.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	/bin/rm -f *.o dgrep
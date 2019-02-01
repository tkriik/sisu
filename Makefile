CC=		clang

STDFLAGS=	-std=c11

DFLAGS=		-D_DEFAULT_SOURCE

OFLAGS=		-O2

WFLAGS=		-Wall \
		-Wextra \
		-Wconversion \
		-Wfloat-equal \
		-Wshadow \
		-Wpointer-arith \
		-Wcast-align \
		-Wstrict-prototypes \
		-Wstrict-overflow=5 \
		-Wwrite-strings \
		-Waggregate-return \
		-Wcast-qual \
		-Wswitch-enum \
		-Wundef \
		-Wunreachable-code \
		-Wuninitialized \
		-Wformat=2 \
		-Wno-format-nonliteral

FFLAGS=		-ftrapv

EXTRAFLAGS=	-Werror \
		-DNDEBUG

CFLAGS=		$(STDFLAGS) $(DFLAGS) $(OFLAGS) $(WFLAGS) $(FFLAGS) $(EXTRAFLAGS)

LDFLAGS=	-lsqlite3

SRC=		main.c \
		db.c \
		file.c \
		log.c \
		scan.c

BIN=		sisu

all: $(SRC)
	$(CC) -o $(BIN) $(CFLAGS) $(SRC) $(LDFLAGS)

debug: OFLAGS = -Og -fno-omit-frame-pointer
debug: EXTRAFLAGS = -g
debug: all

.PHONY: all debug clean

clean:
	rm -f $(BIN)

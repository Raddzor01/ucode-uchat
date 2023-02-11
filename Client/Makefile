CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

all: 
	${CC} ${CFLAGS} -o chat ./src/*.c ${LIBS}

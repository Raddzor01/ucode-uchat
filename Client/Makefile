CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4

GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

OBJDIR = obj
SRCDIR = src
SRCDIRS = $(SRCDIR) $(SRCDIR)/api $(SRCDIR)/lists_functions
UCHAT = ../uchat
DATA_DIR = data

SRC = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/libcjson.a

SSL_DIR = ../libs/openssl
SSL_FILES:= ../local_lib/lib
SSL_INC_FILES:= $(SSL_DIR)

LIB_FLAGS = -lmx -lcjson -lsqlite3 -lssl -lcrypto\
			-L$(LIBMX_DIR) $(LIBMX_NAME)\
			-L$(CJSON_DIR) $(CJSON_NAME)\
			# -L$(SSL_FILES) -I$(SSL_INC_FILES)

all: clean $(UCHAT)

$(UCHAT):
	mkdir $(OBJDIR)
	mkdir $(DATA_DIR)
	$(CC) $(CFLAGS) $(GTK_FLAGS) -c $(SRC)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) $(GTK_LIBS_FLAGS) -o $(UCHAT) $(OBJS) $(LIB_FLAGS)
	# ${GCC} ${GTKFLAGS} -o chat ./src/*.c ${LIBS}

clean:
	rm -f $(OBJS)
	rm -rdf $(OBJDIR)
	rm -rdf $(DATA_DIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all

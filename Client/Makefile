CC = clang
# CFLAGS = -std=gnu11 -pthread -g 
# -Wall -Wextra -Werror -Wpedantic

GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS_FLAGS = `pkg-config --cflags --libs gtk+-3.0`
	
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4

OBJDIR = obj
SRCDIR = src src/request_handlers src/gui
UCHAT = ../ucode-uchat

SRC = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
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
	$(CC) $(CFLAGS) $(GTK_FLAGS) -c $(SRC)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) $(GTK_LIBS_FLAGS) -o $(UCHAT) $(OBJS) $(LIB_FLAGS)
	# ${GCC} ${GTKFLAGS} -o chat ./src/*.c ${LIBS}

clean:
	rm -f $(OBJS)
	rm -rdf $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all

CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4

OBJDIR = obj
SRCDIR = src src/api src/db_functions
SRVR_INFO_DIR = info
DATA_DIR = db/data
UCHAT = ../uchat_server

SRC = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/libcjson.a

LIB_FLAGS = -lmx -lcjson -lsqlite3 -lssl -lcrypto \
			-L$(LIBMX_DIR) $(LIBMX_NAME) \
			-L$(CJSON_DIR) $(CJSON_NAME)

all: clean $(UCHAT)

$(UCHAT):
	mkdir $(SRVR_INFO_DIR)
	mkdir $(OBJDIR)
	# mkdir $(DATA_DIR)
	$(CC) $(CFLAGS) -c $(SRC)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) -o $(UCHAT) $(OBJS) $(LIB_FLAGS)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)
	rm -rdf $(SRVR_INFO_DIR)
	# rm -rdf $(DATA_DIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all

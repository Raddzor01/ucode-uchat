OBJDIR = obj
SRCDIR = src
SERVER_DIR = Server
CLIENT_DIR = Client
SERVER = uchat_server
CLIENT = uchat

LIBMX_DIR = libs/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

CJSON_DIR = libs/cjson
CJSON = $(CJSON_DIR)/cjson.a

SRC = $(SRCDIR)/*.c
OBJS = $(OBJDIR)/*.o

all: clean $(LIBMX) $(CJSON) $(SERVER)

$(LIBMX):
	make -sC $(LIBMX_DIR)
$(CJSON):
	make -sC $(CJSON_DIR)
$(SERVER):
	make -sC $(SERVER_DIR)

$(CLIENT):
	make -sC $(CLIENT_DIR)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR) 

uninstall:
	make -sC $(SERVER_DIR) $@
	make -sC $(CLIENT_DIR) $@
	make -sC $(LIBMX_DIR) $@
	make -sC $(CJSON_DIR) $@
	make clean

reinstall:
	make uninstall
	make all

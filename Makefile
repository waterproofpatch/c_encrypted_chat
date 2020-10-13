# Source code directories.
SRC_DIR=src

SERVER_SRC_DIR=$(SRC_DIR)/server/src
SERVER_INC_DIR=$(SRC_DIR)/server/inc

CLIENT_SRC_DIR=$(SRC_DIR)/client/src
CLIENT_INC_DIR=$(SRC_DIR)/client/inc

SHARED_SRC_DIR=$(SRC_DIR)/shared/src
SHARED_INC_DIR=$(SRC_DIR)/shared/inc

# Library output directories.
LIB_DIR=lib
SERVER_LIB_DIR=$(LIB_DIR)/server
CLIENT_LIB_DIR=$(LIB_DIR)/client

# Standalone executable directories.
BIN_DIR=bin
SERVER_BIN_DIR=$(BIN_DIR)/server
CLIENT_BIN_DIR=$(BIN_DIR)/client

# Directories for documentation.
DOCS_DIRS=html latex docs

# Toolchains.
CC=gcc

# Flags.
# FLAGS=-Wall -lcrypto -lssl
FLAGS=-L/usr/local/opt/openssl/lib -lcrypto -Wall -lssl

# Includes for main driver.
ROOT_MAIN_INCLUDES=-I$(SERVER_INC_DIR) -I$(CLIENT_INC_DIR) -I$(SHARED_INC_DIR)
SERVER_INCLUDES=-I$(SHARED_INC_DIR) -I$(SERVER_INC_DIR)
CLIENT_INCLUDES=-I$(SHARED_INC_DIR) -I$(CLIENT_INC_DIR)

# Build every time.
.PHONY: docs

# Build everything.
all: dirs server client client_main server_main main 

# Client shared library.
client: dirs
	$(CC) $(FLAGS) $(CLIENT_INCLUDES) $(CLIENT_SRC_DIR)/*.c -shared -o $(CLIENT_LIB_DIR)/libclient.so 

# Server shared library.
server: dirs
	$(CC) $(FLAGS) $(SERVER_INCLUDES) $(SERVER_SRC_DIR)/*.c -shared -o $(SERVER_LIB_DIR)/libserver.so 

# Client program (standalone).
client_main: dirs client
	$(CC) $(FLAGS) $(CLIENT_INCLUDES) $(CLIENT_SRC_DIR)/*.c -o $(CLIENT_BIN_DIR)/client.bin 

# Server program (standalone).
server_main: dirs server
	$(CC) $(FLAGS) $(SERVER_INCLUDES) $(SERVER_SRC_DIR)/*.c -o $(SERVER_BIN_DIR)/server.bin 

# Main driver program, to run both server and client at once.
main: dirs server client
	$(CC) $(FLAGS) $(ROOT_MAIN_INCLUDES) -L$(LIB_DIR)/server -L$(LIB_DIR)/client -lserver -lclient $(SRC_DIR)/main.c -o $(BIN_DIR)/main

# Documentation.
docs:
	doxygen

run: main
	$(BIN_DIR)/main

# Create directories needed for building.
dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(SERVER_LIB_DIR)
	mkdir -p $(CLIENT_LIB_DIR)
	mkdir -p $(SERVER_BIN_DIR)
	mkdir -p $(CLIENT_BIN_DIR)

# Clean directories generated from building any of the targets.
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(DOCS_DIRS)

CC = gcc
CFLAGS = -g -Wall -Werror -Iinclude
SRC_DIR = src
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OUTPUT = bin/kv

all: $(OUTPUT)

$(OUTPUT): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)

ready:
	mkdir bin
	mkdir /tmp/db

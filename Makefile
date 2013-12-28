FOO_VERSION=0.99

PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin

CC=gcc
AR=ar
INSTALL=install

PROG=input_edge_to_graph
OBJS=edge.o tree.o

BUILD_DIR = ./build
UNITY_TARGET = $(BUILD_DIR)/test_target

UNITY_SRC_DIR = ./test/src
UNITY_FIXTURE = ./test/fixtures

SRC_FILES  =$(UNITY_SRC_DIR)/unity.c
SRC_FILES +=$(UNITY_FIXTURE)/unity_fixture.c
SRC_FILES += test/all_tests.c

UNITY_INC_DIRS  =-I./
UNITY_INC_DIRS +=-I$(UNITY_SRC_DIR)
UNITY_INC_DIRS +=-I$(UNITY_FIXTURE)

all: $(PROG)

$(PROG): $(OBJS)

install: $(PROG)
	$(INSTALL) -m755 -d $(INSTALL_BIN)
	$(INSTALL) $(PROG) $(INSTALL_BIN)

clean:
	rm -f $(OBJS) $(PROG)

unity_test_edge:
	$(CC) $(CFLAGS) $(UNITY_INC_DIRS) $(SRC_FILES) test/test_edge.c -o $(UNITY_TARGET)
	./$(UNITY_TARGET)

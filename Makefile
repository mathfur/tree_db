FOO_VERSION=0.99

PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin

CC=gcc
AR=ar
INSTALL=install

PROG?=tree_db
OBJS=edge.o tree.o server.o helper.o

BUILD_DIR = ./build
UNITY_TARGET = $(BUILD_DIR)/test_target
PERFORMANCE_TEST_TARGET = $(BUILD_DIR)/test_performance

UNITY_SRC_DIR = ./test/src
UNITY_FIXTURE = ./test/fixtures

SRC_FILES  =$(UNITY_SRC_DIR)/unity.c
SRC_FILES +=$(UNITY_FIXTURE)/unity_fixture.c

UNITY_INC_DIRS  =-I./
UNITY_INC_DIRS +=-I$(UNITY_SRC_DIR)
UNITY_INC_DIRS +=-I$(UNITY_FIXTURE)

TEST_NAMES = $(patsubst %.o,test_%,$(OBJS))
PERFORMANCE_TEST_NAMES = $(patsubst %.o,performance_test_%,$(OBJS))

.PHONY: test $(TEST_NAMES)

ifdef USE_LLDB
  UNITY_PREFIX +=lldb
  CFLAGS +=-g -O0
endif

all: $(PROG)

$(PROG): $(OBJS)

install: $(PROG)
	$(INSTALL) -m755 -d $(INSTALL_BIN)
	$(INSTALL) $(PROG) $(INSTALL_BIN)

uninstall:
	$(RM) $(INSTALL_BIN)/$(PROG)

clean:
	rm -f $(OBJS) $(PROG)

test: clean $(TEST_NAMES) integration_test

$(TEST_NAMES):
	$(CC) $(CFLAGS) $(UNITY_INC_DIRS) $(SRC_FILES) test/$@.c -o $(UNITY_TARGET)
	$(UNITY_PREFIX) ./$(UNITY_TARGET) $(UNITY_SUFFIX)

$(PERFORMANCE_TEST_NAMES):
	$(CC) $(CFLAGS) -DTEST_TAGET=$(patsubst performance_test_%,%,$@) -I./ test/test_performance.c -o $(PERFORMANCE_TEST_TARGET)
	./$(PERFORMANCE_TEST_TARGET)

only_one_respone_target: $(OBJS)
	$(CC) $(CFLAGS) -DONLY_ONE_RESPONSE=1 -I./ $(OBJS) tree_db.c -o $(ONLY_ONE_RESPONE_TARGET)

integration_test_target:
	make clean
	CFLAGS='-DONLY_ONE_RESPONSE=1' make

integration_test: integration_test_target
	ruby test/integration_test.rb

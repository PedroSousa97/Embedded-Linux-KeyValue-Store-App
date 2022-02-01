#
#
#	Makefile for test programs
#
#
#
#
#	Makefile for test programs
#
#
CC = $(CROSS_COMPILE)g++
override CXXFLAGS += -Wall

SRC = $(wildcard KVStoreClient/*.cpp)
OBJ = $(patsubst KVStoreClient/%.cpp, %.o, $(SRC))
EXEC = kvstoreclient

all: $(EXEC)

%.o: KVStoreClient/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC):$(OBJ)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o  $@ $^ -lstdc++



.PHONY: clean
clean:
	rm -f *.o *~ kvstoreclient

.PHONY: install
install:
	cp -f kvstoreclient $(TARGET_DIR)/usr/bin/

.PHONY: uninstall
uninstall:
	rm -f $(TARGET_DIR)/usr/bin/kvstoreclient




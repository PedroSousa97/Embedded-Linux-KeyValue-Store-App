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

SRC = $(wildcard KVStoreAPP/*.cpp)
OBJ = $(patsubst KVStoreAPP/%.cpp, %.o, $(SRC))
EXEC = kvstoreapp

all: $(EXEC)

%.o: KVStoreAPP/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC):$(OBJ)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o  $@ $^ -lleveldb -lstdc++



.PHONY: clean
clean:
	rm -f *.o *~ kvstoreapp

.PHONY: install
install:
	cp -f kvstoreapp $(TARGET_DIR)/usr/bin/

.PHONY: uninstall
uninstall:
	rm -f $(TARGET_DIR)/usr/bin/kvstoreapp




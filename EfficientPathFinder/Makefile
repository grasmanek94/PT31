CFLAGS=-Wall -Werror -pedantic -ggdb -O0 -std=c++98 -I./
CFLAGS_NORMAL=$(CFLAGS)
CFLAGS_MOCK=$(CFLAGS) -I./test


MAIN_SOURCES=main.cxx \
             DynamicGrid.cxx \
             StringSplitter.cxx

TEST_SOURCES=test/main.cxx \
             DynamicGrid.cxx \
             StringSplitter.cxx
             
LIBS=-lpthread
TEST_LIBS=$(LIBS) -lgtest -lgmock -lgmock_main

GCC=g++

.phony: all clean EfficientPathFinder EfficientPathFinderTest 

all: EfficientPathFinder

EfficientPathFinder: $(MAIN_SOURCES)
	@$(GCC) $(CFLAGS_NORMAL) $(MAIN_SOURCES) $(LIBS) -o $@

EfficientPathFinderTest: $(TEST_SOURCES)
	$(GCC) $(CFLAGS_MOCK) $(TEST_SOURCES) $(TEST_LIBS) -o $@

clean:
	@rm -rf EfficientPathFinder EfficientPathFinderTest

test: EfficientPathFinderTest
	@./EfficientPathFinderTest

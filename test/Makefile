CC ?= gcc

# source directory
SRC_DIR = .

# flags for debugging
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -g -DDEBUG
else
CFLAGS += -O3
endif

# flags
LIB_NAME=$(shell basename ../*.so)
CFLAGS += -Wall -Wextra -std=gnu99
LDFLAGS += -L.. -Wl,-rpath=..

# includes and libraries
INCS = -I../src
LIBS = -l:$(LIB_NAME)

# source, object and output
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
OUTPUTS = $(SRC:.c=.bin)

all: $(OUTPUTS)

%.bin: $(OBJ)
	$(CC) $(@:.bin=.o) $(LDFLAGS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $<

clean:
	rm -f $(SRC_DIR)/*.o *.bin

run-tests:
	@for f in *.bin; do valgrind --leak-check=full --show-leak-kinds=all ./$$f; echo; done

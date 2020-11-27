
C_SRCS = $(wildcard *.c)
C_OBJS = $(C_SRCS:.c=.o)
GCC = gcc
GCC_FLAGS = -g -O0 -Wall -Wextra 
BIN = scc

all: $(BIN)

.PHONY: clean

$(BIN): $(C_OBJS)
	$(GCC) -o $@ $(GCC_FLAGS) $(C_OBJS)

%.o: %.c
	$(GCC) -c $< $(GCC_FLAGS) -o $@

clean:
	rm -f *.o
	rm -f *.s
	rm -f $(BIN)

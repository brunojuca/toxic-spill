c_srcfiles := $(wildcard *.c) $(wildcard src/*.c)
objects	:= $(filter %.o,$(patsubst %.c,%.c.o,$(c_srcfiles)))

CC = gcc
CC_FLAGS = -I ./include -O2 -std=c89

debug_flags = 

target = ./toxic-spill
 
all: $(target)

$(notdir $(target)): $(objects)
	echo "Linking "$@"..."
	$(CC) $(objects) -o $@
 
%.c.o: %.c
	echo "Compiling C "$<"..."
	$(CC) $(CC_FLAGS) $(debug_flags) -c $< -o $@

clean:
	rm -rf $(objects) $(target)

.PHONY: all clean

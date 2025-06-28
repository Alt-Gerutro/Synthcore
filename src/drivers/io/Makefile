include ../../../build/build.mk

CFLAGS += -I../../../include

TEMP_DIR = ../../../build/temp/

EXCLUDED =

SRCS = $(filter-out $(EXCLUDED), $(wildcard *.c))
OBJS = $(SRCS:.c=.o)

all: clean $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(TEMP_DIR)/$@

clean:
	rm -f *.o

.SILENT: clean
.PHONY: clean

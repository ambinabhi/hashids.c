CC = gcc
CFLAGS = -g
LDLIBS = -lm

OBJECTS = examples/encoding_decoding_several_numbers.o

hashids.out : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o hashids $(LDLIBS)

%.o : examples/%.c
	$(CC) $(CFLAGS) -c $< $(LDLIBS)

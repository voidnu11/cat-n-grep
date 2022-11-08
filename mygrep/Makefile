TARGET=mygrep
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) $< -o $@

$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

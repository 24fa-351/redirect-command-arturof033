CC = gcc
CFLAGS = -Wall 
SRCS = redir.c redir_func.c
OBJS = redir.o redir_func.o
TARGET = redir

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c redir_func.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

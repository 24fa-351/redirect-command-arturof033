CC = gcc
CFLAGS = -Wall 
SRCS = parsepath.c redir_func.c
OBJS = parsepath.o redir_func.o
TARGET = parsepath

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c redir_func.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

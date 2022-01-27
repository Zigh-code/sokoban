OBJECTS = main.c login.c connect.h logic.c play.h tcp_client.c
CC = gcc


 main.o : main.c 



all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@   

clean :
	del *.o

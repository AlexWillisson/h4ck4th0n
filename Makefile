# The lines of interest to you are SERVER_OBJECTS and CLIENT_OBJECTS, which specify the object files that are required for the server and client, respectively

CC=g++
LD=g++
CCFLAGS=-I. -O2 -g
LDFLAGS=-lSDL -lGLEW -O2 -g
SERVER_TARGET=server
CLIENT_TARGET=client
SERVER_OBJECTS=clcomm.o world.o socket.o object.o vec.o simulate.o
CLIENT_OBJECTS=client.o render.o socket.o vec.o object.o world.o simulate.o

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJECTS)
	$(LD) -o $(SERVER_TARGET) $(LDFLAGS) $(SERVER_OBJECTS)

$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	$(LD) -o $(CLIENT_TARGET) $(LDFLAGS) $(CLIENT_OBJECTS)

%.o: %.cpp
	$(CC) -c $(CCFLAGS) $<

clean:
	rm -rf $(SERVER_TARGET) $(CLIENT_TARGET) $(SERVER_OBJECTS) $(CLIENT_OBJECTS)
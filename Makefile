CC = g++
CFLAGS = -Wall -std=c++11

SRCS = main.cpp maillage.cpp #fonctions_sauvegarde.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)

CC = g++
CFLAGS = -Wall -g
INCLUDES = -I.
#LFLAGS = 
LIBS = -lncurses
SRCS = utils.cpp main.cpp network.cpp neuron.cpp layer.cpp

OBJS = $(SRCS:.cpp=.o)
MAIN = nn

.PHONY: depend clean

all:    $(MAIN) 
	@echo Compiling nn

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN).out $(OBJS) $(LFLAGS) $(LIBS) 

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	$(RM) *.o *.out

depend: $(SRCS) 
	makedepend -Y $(INCLUDES) $^

# END
# DO NOT DELETE

utils.o: utils.h
main.o: network.h layer.h neuron.h utils.h
network.o: network.h layer.h neuron.h utils.h
neuron.o: network.h layer.h neuron.h utils.h
layer.o: network.h layer.h neuron.h

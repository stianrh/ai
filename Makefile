CC = g++
CFLAGS = -Wall -g
INCLUDES = -I.
#LFLAGS = 
LIBS = -lncurses
SRCS = utils.cpp nn.cpp main.cpp

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
nn.o: nn.h utils.h
main.o: nn.h utils.h

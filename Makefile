CC = g++
CFLAGS = -Wall -g
INCLUDES = -I.
#LFLAGS = -L/home/newhall/lib  -L../lib
LIBS = -lncurses
SRCS = utils.c nn.c main.c

OBJS = $(SRCS:.c=.o)
MAIN = nn

.PHONY: depend clean

all:    $(MAIN) 
	@echo Compiling nn

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) 

.c.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS) 
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

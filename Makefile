CC=gcc
CXX=g++
RM=rm -f

FLAGS=	-Wall\
		-pedantic\
		-g\

EXECUTABLE=bin/test

SRCS=	src/rope.c\
		src/test.c

all:	main
		@echo "Build complete"

main:	$(SRCS)
		mkdir -p bin/
		$(CC) -o $(EXECUTABLE) $(SRCS) $(FLAGS)

clean:	$(RM) $(EXECUTABLE)

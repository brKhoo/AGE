CXX = g++
CXXFLAGS = -std=c++14 -Werror -MMD -g
LDLIBS = -lncurses
EXEC = pong

OBJECTS = \
	entity.o \
	movement.o \
	collision.o \
	gameBoard.o \
	gameEngine.o \
	cursesView.o \
	pong.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${LDLIBS}

%.o: %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}

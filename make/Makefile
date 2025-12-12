CXX = g++
CXXFLAGS = -std=c++14 -Werror -MMD -lncurses -g
EXEC = age

OBJECTS = \
    entity.o \
    movement.o \
    collision.o \
    gameBoard.o \
    gameEngine.o \
    cursesView.o \
    testClient.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${CXXFLAGS}

# Generic rule for building any .o from .cc
%.o: %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}

CXX = g++
CXXFLAGS = -std=c++14 -Werror -MMD -g
LDFLAGS = -lncurses
EXEC = snake

OBJECTS = \
    snake.o \
    entity.o \
    movement.o \
    collision.o \
    gameBoard.o \
    gameEngine.o \
    cursesView.o

DEPENDS = $(OBJECTS:.o=.d)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Generic rule for building any .o from .cc
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)

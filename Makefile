CXX = g++
CXXFLAGS = -std=c++14 -Werror -MMD -g
LDLIBS = -lncurses

EXEC_SNAKE = snake
EXEC_PONG  = pong

COMMON_OBJECTS = \
	entity.o \
	movement.o \
	collision.o \
	gameBoard.o \
	gameEngine.o \
	cursesView.o

SNAKE_OBJECTS = $(COMMON_OBJECTS) snake.o
PONG_OBJECTS  = $(COMMON_OBJECTS) pong.o

OBJECTS = $(SNAKE_OBJECTS) $(PONG_OBJECTS)
DEPENDS = $(OBJECTS:.o=.d)

all: $(EXEC_SNAKE) $(EXEC_PONG)

$(EXEC_SNAKE): $(SNAKE_OBJECTS)
	$(CXX) $(SNAKE_OBJECTS) -o $@ $(LDLIBS)

$(EXEC_PONG): $(PONG_OBJECTS)
	$(CXX) $(PONG_OBJECTS) -o $@ $(LDLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC_SNAKE) $(EXEC_PONG)

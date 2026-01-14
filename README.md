# AGE — Simple ASCII Physics Engine

AGE is a simple ASCII-based physics/game engine written in C++ for terminal/console applications. It includes basic collision handling, movement, input processing, and example game demos (e.g., snake, pong). This project is designed for interacting with physics simulations, game loops, and ASCII rendering in C++.

---

## Features

- ASCII rendering for simple terminal visuals
- Physics and collision detection
- Input handling via ncurses library
- Multiple demo/game examples (snake, pong, etc.)
- Modular engine components for entities, movement, and game state

---

## Built With

- **C++** — core logic
- **ncurses / curses** — terminal UI input & output
- **Make** — build automation

---

## Getting Started

### Prerequisites

To build and run AGE, you need:

- A C++17-compatible compiler (e.g., `g++`, `clang++`)
- `make`
- `ncurses` development libraries installed

On **Linux**:

```bash
sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev
```

### Setup
Clone the repository and build the executables.
```
git clone https://github.com/brKhoo/AGE.git
cd AGE
make
```
Then run the demos:
```
./age
./snake
./pong
```

### Architecture
- The engine is organized into several core systems:
- Game Loop: Updates state and renders frames
- Entities: Game objects with position/velocity, e.g., shapes, players
- Physics: Movement and collision detection (collision.cc/h)
- Input: Terminal input using curses (cursesInput.h)
- Rendering: ASCII drawing in the console (cursesView.cc/h)
- Examples: Demo logic in files like snake.cc, pong.cc

This modular design makes it easier to extend or reuse parts of the engine.

### Example Demos
- AGE includes simple interactive games to demonstrate the engine:
- Snake: Classic snake game in ASCII
- Pong: Basic pong gameplay
- Demo: A general example runner showing engine capabilities

### Future Additions
- Add more physics examples (rigid bodies, forces)
- Refactor engine into reusable library modules
- Add unit tests

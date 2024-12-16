A **C++** application for generating and solving mazes using **Eller's algorithm** for maze generation and **Breadth-First Search (BFS)** or **Depth-First Search (DFS)** for pathfinding. The graphical interface is implemented using **Qt6 Widgets**.

---

## Features

- **Maze Generation**:
  - Implements **Eller's algorithm** to generate random mazes with configurable sizes.
  - Supports rotation of the maze grid.

- **Pathfinding**:
  - Pathfinding algorithms: **BFS** (Breadth-First Search) and **DFS** (Depth-First Search).
  - Visualizes the solution path from the start to the goal.

- **Graphical Interface**:
  - Interactive GUI built with **Qt6 Widgets**.
  - Keyboard and button controls to interact with the maze.

- **Customizable Maze Size**:
  - Pass the maze size through command-line arguments.

---

## Requirements

- **C++20** compiler (e.g., GCC, Clang, MSVC).
- **CMake 3.20** or higher.
- **Qt6** libraries (Widgets, Core, and Gui).

---

## Build Instructions

### 1. Install Qt6
Ensure **Qt6** is installed and available on your system.

### 2. Clone the Repository

```bash
git clone https://github.com/RusikPro/MazeGameCpp.git
cd MazeGame
```

### 3. Build the Project

Run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Run the Game

After building, run the executable:

```bash
./MazeGame <maze_size>
```

- Replace `<maze_size>` with the desired size of the maze (e.g., `100`).
- If no size is specified, the default size (`DEFAULT_MAZE_SIZE` - 50) will be used.
- If the size is bigger than 100 or lower than 0, the program won't start.

Examples:
```bash
./MazeGame 100
```

---

## Usage

### Controls

- **Arrow Keys**: Move the player through the maze.
- **P Key**: Visualize the shortest path to the goal (toggle visualization).
- **G Key**: Generate a new maze with the same size.

### Buttons (Displayed in the GUI)

- **Generate**: Generates a new maze.
- **Find Path**: Visualizes the solution path to the goal.

---

## Command-Line Arguments

Pass the maze size as a command-line argument:

```bash
./MazeGame 50
```

This command generates a `50 x 50` maze.

---

## Project Structure

The project is modular, with components organized as follows:

```plaintext
MazeGame/
├── project/
│   ├── common/                       # Common utilities
│   │   ├── include/
│   │   │   └── common/
│   │   │       ├── Constants.h       # Global constants
│   │   │       ├── Point.h           # Struct for grid points
│   │   │       └── Timer.h           # Timer utility for performance measurements
│   │   ├── src/
│   │   │   └── FlexibleIterator.cpp  # Flexible iterator implementation
│   │   └── CMakeLists.txt            # CMake configuration for common module
│   ├── maze/                         # Maze generation logic
│   │   ├── include/
│   │   │   └── maze/
│   │   │       └── Maze.h            # Maze class declaration
│   │   ├── src/
│   │   │   └── Maze.cpp              # Maze generation logic (Eller's algorithm)
│   │   └── CMakeLists.txt            # CMake configuration for maze module
│   ├── pathfinding/                  # Pathfinding algorithms
│   │   ├── include/
│   │   │   └── pathfinding/
│   │   │       └── PathFinder.h      # Pathfinding algorithms (BFS/DFS)
│   │   ├── src/
│   │   │   └── PathFinder.cpp        # Implementation of BFS/DFS
│   │   └── CMakeLists.txt            # CMake configuration for pathfinding module
│   ├── gui/                          # Graphical interface for the game
│   │   ├── include/
│   │   │   └── gui/
│   │   │       └── GameWidget.h      # GUI class for maze interaction
│   │   ├── src/
│   │   │   └── GameWidget.cpp        # Implementation of the GUI
│   │   └── CMakeLists.txt            # CMake configuration for gui module
│   ├── main.cpp                      # Main entry point
│   └── CMakeLists.txt                # CMake configuration for project root
├── CMakeLists.txt                    # Root CMake configuration
└── README.md                         # Project documentation
```

---

## Code Highlights

### Maze Generation (Eller's Algorithm)

The `Maze` class generates a maze using **Eller's algorithm**, ensuring that the maze is perfect (exactly one path between any two points).

```cpp
Maze::Maze ( int _size )
    :   m_size( _size )
    ,   m_grid( _size, Rooms( _size ) )
{
    validateSize();
    reset();
    generateMaze();
}
```

### Pathfinding (BFS/DFS)

The `PathFinder` class implements **BFS** and **DFS** algorithms to solve the maze. BFS is used by default.

```cpp
PathFinder::Path PathFinder::solve ( Point _start, Point _goal, bool _useBFS )
{
    std::unique_ptr<Frontier> frontier = _useBFS
        ? std::make_unique<QueueFrontier>()
        : std::make_unique<StackFrontier>();

    // Search for the goal
}
```

### Graphical Interface

The `GameWidget` class provides the interactive GUI with player movement, path visualization, and maze regeneration.

```cpp
GameWidget::GameWidget(int _mazeSize, QWidget *parent)
    : QWidget(parent), maze(_mazeSize)
{
    setupScene();
    generateNewMaze();
    drawMaze();
}
```

---

## Screenshots

### 1. **Generated Maze**

For 50 rooms:
<img width="863" alt="image" src="https://github.com/user-attachments/assets/5ec38825-5448-40cb-938a-bd776bc68a39" />

For 100 rooms:
<img width="1061" alt="image" src="https://github.com/user-attachments/assets/4ed11f05-c599-4ad8-bc65-fd38c573b0aa" />


### 2. **Path Visualization**

Pressing the "Find Path" button (or P on the keyboard) finds a path from your current position, using BFS.
<img width="1060" alt="image" src="https://github.com/user-attachments/assets/1f558519-c072-4148-9aa0-fb75f0d54825" />


### 3. **New Maze Generation**

Pressing the "Generate" button (or G on the keyboard) creates a new maze.
<img width="862" alt="image" src="https://github.com/user-attachments/assets/9efb040f-734a-42d2-855f-8b52ee5c9caf" />

### 4. **Benchmarks in console**

<img width="518" alt="image" src="https://github.com/user-attachments/assets/32111271-e664-4569-a4c1-dda8b6c2e4f5" />

---

## Future Improvements

- Add support for more pathfinding algorithms (e.g., A*).
- Allow saving and loading mazes.
- Fix timer to measure the player's time to solve the maze to restart after the maze is regenerated.
- Refactor gui module.

---

## License

This project is licensed under the **MIT License**.

---

## Contributors

- **Ruslan Arkhyryeyev**

Feel free to contribute! Fork the repository and submit pull requests.

---

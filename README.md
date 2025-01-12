A **C++** application for generating and solving mazes using **Eller's** and **Kruskal's** algorithms for maze generation and **Breadth-First Search (BFS)** or **Depth-First Search (DFS)** for pathfinding. The graphical interface is implemented using **Qt6 Widgets**.

---

## Features

- **Maze Generation**:
  - Implements **Eller's algorithm** to generate random mazes with configurable sizes.
  - Implements **Kruskal's algorithm** to generate random mazes with configurable sizes.
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
│   ├── common/                         # Common utilities
│   │   ├── include/
│   │   │   └── common/
│   │   │       ├── Constants.h         # Global constants
│   │   │       ├── FlexibleIterator.h  # Forward/Backward indexer
│   │   │       ├── Point.h             # Struct for grid points
│   │   │       └── Timer.h             # Timer utility for performance measurements
│   │   ├── src/
│   │   │   └── FlexibleIterator.cpp    # Flexible iterator implementation
│   │   └── CMakeLists.txt              # CMake configuration for common module
│   ├── maze/                           # Maze generation logic
│   │   ├── include/
│   │   │   └── maze/
│   │   │       └── IMaze.h             # IMaze class interface
│   │   │       └── IMazeFactory.h      # IMazeFactor class
│   │   ├── src/
│   │   │   ├── BaseMaze.h              # Basic IMaze interface implementation (still abstract)
│   │   │   ├── BaseMaze.cpp            # Sources
│   │   │   ├── EllerMaze.h             # Eller's maze generation full implementation
│   │   │   ├── EllerMaze.cpp           # Sources
│   │   │   ├── KruskalMaze.h           # Kruskal's maze generation full implementation
│   │   │   ├── KruskalMaze.cpp         # Sources
│   │   │   ├── MazeFactory.h           # IMazeFactory interface implementation
│   │   │   └── MazeFactory.cpp         # Sources + createFactory static method implementation
│   │   └── CMakeLists.txt              # CMake configuration for maze module
│   ├── pathfinding/                    # Pathfinding algorithms
│   │   ├── include/
│   │   │   └── pathfinding/
│   │   │       └── PathFinder.h        # Pathfinding algorithms (BFS/DFS)
│   │   ├── src/
│   │   │   ├── Frontier.h              # Internal structure for search algorithms
│   │   │   ├── Frontier.cpp            # Implementation of internal structure for search algorithms
│   │   │   └── PathFinder.cpp          # Implementation of BFS/DFS
│   │   └── CMakeLists.txt              # CMake configuration for pathfinding module
│   ├── gui/                            # Graphical interface for the game
│   │   ├── include/
│   │   │   └── gui/
│   │   │       └── GameWidget.h        # GUI class for maze interaction
│   │   ├── src/
│   │   │   └── GameWidget.cpp          # Implementation of the GUI
│   │   └── CMakeLists.txt              # CMake configuration for gui module
│   ├── main.cpp                        # Main entry point
│   └── CMakeLists.txt                  # CMake configuration for project root
├── CMakeLists.txt                      # Root CMake configuration
└── README.md                           # Project documentation
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
PathFinder::Path PathFinder::solve ( Point const & _start, Point const & _goal, bool _useBFS )
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

#### For 50 rooms
```bash
./MazeGame 50
```
<img width="844" alt="image" src="https://github.com/user-attachments/assets/bb514bd7-3c33-43a9-b704-dd2dc16b866a" />


#### For 100 rooms
```bash
./MazeGame 100
```
<img width="1167" alt="image" src="https://github.com/user-attachments/assets/9a1b3b4c-4a64-47f6-b448-68ce0dc1aa76" />


### 2. **New Maze Generation**

Clicking the "Generate" (either Kruskal or Eller) button (or K (Kruskal) or E (Eller) on the keyboard) creates a new maze.
<img width="1158" alt="image" src="https://github.com/user-attachments/assets/7bf252fe-6c4b-4a14-b537-1de3d410235e" />


### 3. **Path Visualization**

Pressing the "Find Path" (either BFS or DFS) button (or B (BFS) or D (DFS) on the keyboard) finds a path from your current position.

<img width="1160" alt="image" src="https://github.com/user-attachments/assets/15a3d574-22cb-4c11-9717-31cc1853d843" />

### 4. **Benchmarks in console**

Benchmarks are logged in the console and show the performance of different generation and path finding algorithms applied.

<img width="351" alt="image" src="https://github.com/user-attachments/assets/13c62723-813d-4021-a98c-2081fa1a4eef" />


---

## Future Improvements

- Add support for more pathfinding algorithms (e.g., A*).
- Refactor gui module.
- Make maze functionalities modules as a static library.
- Extend the project into several separate maze clients: gui, http, python lib, and more.
  - http API (pistache.io) that will serve mazes on requests;
  - Ability to save maze in a database;
  - maze module as a portable python library;
  - gui - facilitate changes to different gui providers.

---

## License

This project is licensed under the **MIT License**.

---

## Contributors

- **Ruslan Arkhyryeyev**

Feel free to contribute! Fork the repository and submit pull requests.

---

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Maze constraints
constexpr int MAX_MAZE_SIZE = 100;           // Maximum allowed size for the maze
constexpr int DEFAULT_MAZE_SIZE = 50;        // Default maze size if none provided
constexpr int MAX_CELLS_FOR_SCALING = 60;    // Maximum cells for dynamic scaling
constexpr int MAX_RENDER_SIZE = 700;        // Maximum render size of the maze

// Scene and rendering
constexpr int SCENE_PADDING = 15;            // Padding around the scene
constexpr int BORDER_WIDTH = 2;              // Border width for outer maze walls
constexpr int TIMER_INTERVAL_MS = 1000;      // Timer interval in milliseconds
constexpr int CELL_MARGIN_FACTOR = 4;        // Factor for cell margin when placing player and destination

// Player and destination
constexpr int PLAYER_COLOR_RED = 255;        // Red color for the player
constexpr int PLAYER_COLOR_GREEN = 0;        // Green color for the player
constexpr int PLAYER_COLOR_BLUE = 0;         // Blue color for the player
constexpr int DESTINATION_COLOR_GREEN = 255; // Green color for the destination

// Maze generation probabilities
constexpr int HORIZONTAL_MERGE_PROBABILITY = 45;  // Probability of merging sets horizontally
constexpr int VERTICAL_CONNECTION_PROBABILITY = 45; // Probability of creating vertical connections

// Walls
constexpr int TOP_WALL = 0;
constexpr int RIGHT_WALL = 1;
constexpr int BOTTOM_WALL = 2;
constexpr int LEFT_WALL = 3;

#endif // CONSTANTS_H

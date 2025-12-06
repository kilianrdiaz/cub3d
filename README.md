# cub3d - 42 Barcelona
**cub3d** is a simple 3D maze renderer / game engine written in C, in this case recreating the game mode “Escape from the Sewers” from the Spider-Man Plug and Play console created by Jakks Pacific. This project is part of the learning curriculum of the 42 Campus Common Core and is originally inspired by the classic first-person shooter Wolfenstein 3D.

The engine uses ray-casting techniques on a 2D map to render a pseudo-3D environment, allowing the player to “walk through” a maze and navigate from a first-person perspective.

## Main Features
**Ray-casting based rendering** — walls, sprites, and the maze layout described in a 2D map file are projected into a 3D view using ray-casting and trigonometry. Wall collision is also implemented as part of the core gameplay. Darkness and lighting details are added to stay faithful to the original game.

**Map description via .cub files and texture files** — the engine parses a configuration file (with the .cub extension) that defines wall textures for the four directions (north/south/east/west), floor and ceiling colors, as well as the 2D map layout including the player’s starting position, active bomb locations, and enemy spawn points. Main textures and additional sprites are provided to the program through the ./textures/ directory.

**Basic movement & additional features** — the player can move and rotate using the WASD keys, turn the camera on the X-axis using the mouse, and interact with map elements using SPACE (to attack enemies or defuse active bombs). The health bar, minimap, bomb counter, countdown timer, and soundtrack implementation are based on the original game. Additional features such as a scoreboard and a pause key (P) are ideas of my own.

/* TO-DO 
*
* - Installation, compilation and execution
* - Game control keys
* - Game objective and explanation
* - Subject.txt
* - Screenshots and videos

**Graphics handled via a Minilibx** — the project uses MiniLibX, a minimal graphics library commonly used in 42-school projects, to open a window and render the game scene.

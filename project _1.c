#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define TILE_SIZE (WINDOW_WIDTH / MAP_WIDTH)

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void initializeSDL()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Raycasting Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void handleEvents(bool *quit)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *quit = true;
        }
    }
}

void renderScene()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            int tile = map[x][y];

            SDL_Rect rect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if (tile == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    bool quit = false;

    initializeSDL();

    while (!quit) {
        handleEvents(&quit);
        renderScene();
    }

    cleanup();

    return 0;
}


#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV_ANGLE 60
#define NUM_RAYS SCREEN_WIDTH

struct Camera {
    float x;
    float y;
    float angle;
};

struct Camera camera;

void handleInput(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                camera.angle -= 0.1; // Adjust rotation angle
                break;
            case SDLK_RIGHT:
                camera.angle += 0.1; // Adjust rotation angle
                break;
            // Handle other key events for movement
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        // Adjust rotation angle based on mouse movement
        camera.angle += (float)event.motion.xrel / 100.0;
    }
}

void render() {
    // Perform raycasting with the updated camera angle
    for (int i = 0; i < NUM_RAYS; i++) {
        float rayAngle = (camera.angle - FOV_ANGLE / 2.0) + ((float)i / NUM_RAYS) * FOV_ANGLE;
        // Perform ray casting calculations
        // ...
        // Render walls
        // ...
    }
}

int main() {
    // Initialize SDL and create a window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Camera Rotation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set initial camera position and angle
    camera.x = 0.0;
    camera.y = 0.0;
    camera.angle = 0.0;

    // Game loop
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
            handleInput(event);
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the scene
        render();

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    return 0;
}


#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV_ANGLE 60
#define NUM_RAYS SCREEN_WIDTH
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 50

struct Camera {
    float x;
    float y;
    float angle;
};

struct Camera camera;

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void handleInput(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                // Move forward
                camera.x += cosf(camera.angle) * 5.0;
                camera.y += sinf(camera.angle) * 5.0;
                // Check for collision
                if (map[(int)(camera.y / TILE_SIZE)][(int)(camera.x / TILE_SIZE)] == 1) {
                    // Collision occurred, move player back
                    camera.x -= cosf(camera.angle) * 5.0;
                    camera.y -= sinf(camera.angle) * 5.0;
                }
                break;
            case SDLK_s:
                // Move backward
                camera.x -= cosf(camera.angle) * 5.0;
                camera.y -= sinf(camera.angle) * 5.0;
                // Check for collision
                if (map[(int)(camera.y / TILE_SIZE)][(int)(camera.x / TILE_SIZE)] == 1) {
                    // Collision occurred, move player back
                    camera.x += cosf(camera.angle) * 5.0;
                    camera.y += sinf(camera.angle) * 5.0;
                }
                break;
            case SDLK_a:
                // Strafe left
                camera.x -= cosf(camera.angle + M_PI / 2.0) * 5.0;
                camera.y -= sinf(camera.angle + M_PI / 2.0) * 5.0;
                // Check for collision
                if (map[(int)(camera.y / TILE_SIZE)][(int)(camera.x / TILE_SIZE)] == 1) {
                    // Collision occurred, move player back
                    camera.x += cosf(camera.angle + M_PI / 2.0) * 5.0;
                    camera.y += sinf(camera.angle + M_PI / 2.0) * 5.0;
                }
                break;
            case SDLK_d:
                // Strafe right
                camera.x += cosf(camera.angle + M_PI / 2.0) * 5.0;
                camera.y += sinf(camera.angle + M_PI / 2.0) * 5.0;
                // Check for collision
                if (map[(int)(camera.y / TILE_SIZE)][(int)(camera.x / TILE_SIZE)] == 1) {
                    // Collision occurred, move player back
                    camera.x -= cosf(camera.angle + M_PI / 2.0) * 5.0;
                    camera.y -= sinf(camera.angle + M_PI / 2.0) * 5.0;
                }
                break;
            // Handle other key events for rotation
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        // Adjust rotation angle based on mouse movement
        camera.angle += (float)event.motion.xrel / 100.0;
    }
}

void render() {
    // Perform raycasting with the updated camera angle
    for (int i = 0; i < NUM_RAYS; i++) {
        float rayAngle = (camera.angle - FOV_ANGLE / 2.0) + ((float)i / NUM_RAYS) * FOV_ANGLE;
        // Perform ray casting calculations
        // ...
        // Render walls
        // ...
    }
    // Render player
    // ...
}

int main() {
    // Initialize SDL and create a window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Camera Movement and Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set initial camera position and angle
    camera.x = 0.0;
    camera.y = 0.0;
    camera.angle = 0.0;

    // Game loop
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
            handleInput(event);
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the scene
        render();

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>

#define MAX_MAP_SIZE 100

// Function to read a map file and populate the map array
void parseMapFile(const char* filename, int map[MAX_MAP_SIZE][MAX_MAP_SIZE], int* width, int* height) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    int row = 0;
    char line[MAX_MAP_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (row >= MAX_MAP_SIZE) {
            printf("Map size exceeds maximum limit\n");
            exit(1);
        }

        int col = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            if (col >= MAX_MAP_SIZE) {
                printf("Map size exceeds maximum limit\n");
                exit(1);
            }

            map[row][col] = atoi(token);
            col++;
            token = strtok(NULL, ",");
        }

        if (col > *width) {
            *width = col;
        }
        row++;
    }

    *height = row;

    fclose(file);
}

int main() {
    int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int width = 0, height = 0;

    // Replace "map.txt" with the path to your map file
    parseMapFile("map.txt", map, &width, &height);

    // Print the loaded map
    printf("Map:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

#define WALL_TEXTURE_PATH "wall.png"
#define GROUND_TEXTURE_PATH "ground.png"
#define CEILING_TEXTURE_PATH "ceiling.png"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* wallTexture = NULL;
SDL_Texture* groundTexture = NULL;
SDL_Texture* ceilingTexture = NULL;

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void loadTextures() {
    wallTexture = IMG_LoadTexture(renderer, WALL_TEXTURE_PATH);
    if (!wallTexture) {
        printf("Failed to load wall texture: %s\n", SDL_GetError());
        exit(1);
    }

    groundTexture = IMG_LoadTexture(renderer, GROUND_TEXTURE_PATH);
    if (!groundTexture) {
        printf("Failed to load ground texture: %s\n", SDL_GetError());
        exit(1);
    }

    ceilingTexture = IMG_LoadTexture(renderer, CEILING_TEXTURE_PATH);
    if (!ceilingTexture) {
        printf("Failed to load ceiling texture: %s\n", SDL_GetError());
        exit(1);
    }
}

void renderMap() {
    SDL_Rect wallRect = {0, 0, WINDOW_WIDTH / MAP_WIDTH, WINDOW_HEIGHT / MAP_HEIGHT};
    SDL_Rect groundRect = {0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2};
    SDL_Rect ceilingRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT / 2};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            if (map[x][y] == 1) {
                wallRect.x = x * (WINDOW_WIDTH / MAP_WIDTH);
                wallRect.y = y * (WINDOW_HEIGHT / MAP_HEIGHT);
                SDL_RenderCopy(renderer, wallTexture, NULL, &wallRect);
            }
        }
    }

    SDL_RenderCopy(renderer, groundTexture, NULL, &groundRect);
    SDL_RenderCopy(renderer, ceilingTexture, NULL, &ceilingRect);

    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Texture Mapping", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        return 1;
    }

    loadTextures();

    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        renderMap();
    }

    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(ceilingTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}



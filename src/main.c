// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2026 jgabaut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

void free_matrix(int** matrix, size_t height)
{
    for (size_t y = 0; y < height; y++) {
        free(matrix[y]);
    }
    free(matrix);
}

void clear_matrix(int** matrix, size_t height, size_t width)
{
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            matrix[y][x] = 0;
        }
    }
}

void draw_matrix(int** matrix, size_t height, size_t width, size_t cell_size)
{
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Color c = BLACK;
            if (matrix[y][x]) {
                c = RED;
            }
            DrawRectangle(x * cell_size, y * cell_size, cell_size, cell_size, c);
        }
    }
}

int count_neighbours(int **matrix, int height, int width, int y, int x) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue; // skip the cell itself

            int ny = y + dy;
            int nx = x + dx;

            // bounds check
            if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                if (matrix[ny][nx]) {
                    count++;
                }
            }
        }
    }

    return count;
}

void update_matrix(int **matrix, size_t height, size_t width) {
    // allocate a new matrix for next generation
    int **next = calloc(height, sizeof(int*));
    for (size_t y = 0; y < height; y++) {
        next[y] = calloc(width, sizeof(int));
    }

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            int nc = count_neighbours(matrix, height, width, y, x);
            int cell = matrix[y][x];

            if (cell) {
                // Survival: 2 or 3 neighbors
                if (nc == 2 || nc == 3) next[y][x] = 1;
                else next[y][x] = 0;
            } else {
                // Birth: exactly 3 neighbors
                if (nc == 3) next[y][x] = 1;
            }
        }
    }

    // copy next into matrix
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            matrix[y][x] = next[y][x];
        }
        free(next[y]);
    }
    free(next);
}

int main(int argc, char** argv)
{
    int height = 600;
    int width = 800;
    InitWindow(width, height, "Game of Life");
    size_t cell_size = 10;
    int matrix_height = height / cell_size;
    int matrix_width = width / cell_size;
    int** matrix = calloc(matrix_height, sizeof(int*));
    for (size_t y = 0; y < matrix_height; y++) {
        matrix[y] = calloc(matrix_width, sizeof(int));
    }

    Vector2 mouse = {0};
    bool live = false;
    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_L)) {
            live = !live;
        }
        mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int y = (int)mouse.y / cell_size;
            int x = (int)mouse.x / cell_size;
            matrix[y][x] = !matrix[y][x];
        }
        if (live) update_matrix(matrix, matrix_height, matrix_width);
        BeginDrawing();
        ClearBackground(WHITE);
        draw_matrix(matrix, matrix_height, matrix_width, cell_size);
        EndDrawing();
    }

    free_matrix(matrix, matrix_height);
    return 0;
}

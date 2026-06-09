#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 60

// 2D Array acting as our graphics drawing canvas
char canvas[ROWS][COLS];

// Shape tracking variables to support Modify and Delete operations
int activeObjType = 0; // 1: Line, 2: Rect, 3: Circle, 4: Triangle
int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0;

// Fills the grid framework with the background canvas character
void clearCanvas() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
        }
    }
}

// Safely plots a pixel, ensuring coordinates stay inside array bounds
void drawPixel(int x, int y, char ch) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = ch;
    }
}

// Displays the complete picture matrix directly to the standard output terminal
void displayCanvas() {
    // Clear screen command to mimic interactive frame updates
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("=== 2D GRAPHICS EDITOR (Canvas Grid: %dx%d) ===\n\n", COLS, ROWS);
    
    // Render out the array contents line-by-line
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            putchar(canvas[r][c]);
        }
        putchar('\n');
    }
    printf("\n");
}

// Bresenham's Line Algorithm for clean pixel steps
void drawLine(int x0, int y0, int x1, int y1, char ch) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        drawPixel(x0, y0, ch);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Draws a structural bounding rectangle outline using 4 perimeter lines
void drawRectangle(int x, int y, int w, int h, char ch) {
    drawLine(x, y, x + w - 1, y, ch);                 // Top edge
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, ch); // Bottom edge
    drawLine(x, y, x, y + h - 1, ch);                 // Left edge
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, ch); // Right edge
}

// Midpoint Circle Algorithm to avoid grid mapping distortion
void drawCircle(int xc, int yc, int r, char ch) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    while (y >= x) {
        drawPixel(xc + x, yc + y, ch);
        drawPixel(xc - x, yc + y, ch);
        drawPixel(xc + x, yc - y, ch);
        drawPixel(xc - x, yc - y, ch);
        drawPixel(xc + y, yc + x, ch);
        drawPixel(xc - y, yc + x, ch);
        drawPixel(xc + y, yc - x, ch);
        drawPixel(xc - y, yc - x, ch);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// Draws a triangle layout by mapping out lines across three coordinate vertices
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    drawLine(x1, y1, x2, y2, ch);
    drawLine(x2, y2, x3, y3, ch);
    drawLine(x3, y3, x1, y1, ch);
}

// Helper to handle structural state erasure
void removeActiveObject() {
    if (activeObjType == 1) drawLine(p1, p2, p3, p4, '_');
    else if (activeObjType == 2) drawRectangle(p1, p2, p3, p4, '_');
    else if (activeObjType == 3) drawCircle(p1, p2, p3, '_');
    else if (activeObjType == 4) drawTriangle(p1, p2, p3, p4, p5, p6, '_');
}

int main() {
    int choice;
    clearCanvas();

    while (1) {
        displayCanvas();
        
        // Render Output Dashboard Options 
        printf("--- Dashboard Options ---\n");
        printf("1. Add Line (x1 y1 x2 y2)\n");
        printf("2. Add Rectangle (x y width height)\n");
        printf("3. Add Circle (center_x center_y radius)\n");
        printf("4. Add Triangle (x1 y1 x2 y2 x3 y3)\n");
        printf("5. Delete Current Object\n");
        printf("6. Modify Current Object\n");
        printf("7. Exit Application\n");
        printf("Select Action (1-7): ");
        
        if (scanf("%d", &choice) != 1) {
            // Buffer safety to clean up invalid character input entry loops
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 1) {
            printf("Enter Line Coordinates: ");
            scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
            activeObjType = 1;
            drawLine(p1, p2, p3, p4, '*');
        } 
        else if (choice == 2) {
            printf("Enter Rectangle Coordinates: ");
            scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
            activeObjType = 2;
            drawRectangle(p1, p2, p3, p4, '*');
        } 
        else if (choice == 3) {
            printf("Enter Circle Coordinates: ");
            scanf("%d %d %d", &p1, &p2, &p3);
            activeObjType = 3;
            drawCircle(p1, p2, p3, '*');
        } 
        else if (choice == 4) {
            printf("Enter Triangle Coordinates: ");
            scanf("%d %d %d %d %d %d", &p1, &p2, &p3, &p4, &p5, &p6);
            activeObjType = 4;
            drawTriangle(p1, p2, p3, p4, p5, p6, '*');
        } 
        else if (choice == 5) {
            removeActiveObject();
            activeObjType = 0; // Clear memory footprint tracking
        } 
        else if (choice == 6) {
            if (activeObjType == 0) {
                printf("No object exists to modify! Press Enter to return.");
                while (getchar() != '\n'); getchar();
                continue;
            }
            // Wipe coordinates clean first
            removeActiveObject();
            
            // Get new properties
            if (activeObjType == 1) {
                printf("Enter New Line Coordinates: ");
                scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
                drawLine(p1, p2, p3, p4, '*');
            } else if (activeObjType == 2) {
                printf("Enter New Rectangle Coordinates: ");
                scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
                drawRectangle(p1, p2, p3, p4, '*');
            } else if (activeObjType == 3) {
                printf("Enter New Circle Coordinates: ");
                scanf("%d %d %d", &p1, &p2, &p3);
                drawCircle(p1, p2, p3, '*');
            } else if (activeObjType == 4) {
                printf("Enter New Triangle Coordinates: ");
                scanf("%d %d %d %d %d %d", &p1, &p2, &p3, &p4, &p5, &p6);
                drawTriangle(p1, p2, p3, p4, p5, p6, '*');
            }
        } 
        else if (choice == 7) {
            printf("Exiting System...\n");
            break;
        }
    }
    return 0;
}

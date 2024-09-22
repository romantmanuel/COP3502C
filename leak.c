#include <stdio.h>
#include <stdlib.h>

#define inscription 2000

// Structure of a grid where a point will be
typedef struct {
    int row;
    int col;
} Point;

// Structure for a queue
typedef struct {
    Point * items;
    int front;
    int rear;
    int size;
} Queue;

// Variable declarations
char grid[inscription][inscription];
int rows, columns;
int neighbor[inscription][inscription];
int neighbor_size[inscription * inscription];
int neighbor_count = 0;

// Allows us to move up, down, left, right
int adjacentRow[] = {-1, 1, 0, 0};
int adjacentColumn[] = {0, 0, -1, 1};

// My function prototypes
Queue * createQueue(int capacity);
void enqueue(Queue * que, Point p);
Point dequeue(Queue * que);
int isEmpty(Queue* que);
void freeQueue(Queue * que);
void floodFill(int start_row, int start_col);
long long calculatePairs(int n);
long long findMaxMagic();

int main() {
    // Gets user input
    scanf("%d %d", &rows, &columns);

    for (int i = 0; i < rows; i++) {
        scanf("%s", grid[i]);
    }

    for (int i = 0; i < inscription; i++) {
        for (int j = 0; j < inscription; j++) {
            neighbor[i][j] = -1;
        }
    }

    // Does flood fill for every free point
    for (int currentRow = 0; currentRow < rows; currentRow++) {
        for (int currentColumn = 0; currentColumn < columns; currentColumn++) {
            if (grid[currentRow][currentColumn] == '.' && neighbor[currentRow][currentColumn] == -1) {
                floodFill(currentRow, currentColumn);
            }
        }
    }

    // Finds the max magic then prints the result
    long long max_magic = findMaxMagic();
    printf("%lld\n", max_magic);

    return 0;
}

// Queue functions; create queue, enqueue, dequeue
Queue * createQueue(int capacity) {
    Queue * que = (Queue*)malloc(sizeof(Queue));
    que->items = (Point*)malloc(capacity * sizeof(Point));
    que->front = 0;
    que->rear = -1;
    que->size = 0;
    return que;
}

void enqueue(Queue * que, Point p) {
    que->rear = (que->rear + 1) % (rows * columns);
    que->items[que->rear] = p;
    que->size++;
}

Point dequeue(Queue * que) {
    Point p = que->items[que->front];
    que->front = (que->front + 1) % (rows * columns);
    que->size--;
    return p;
}

// Makes sure queue is empty
int isEmpty(Queue* que) {
    return que->size == 0;
}

// Frees memory
void freeQueue(Queue * que) {
    free(que->items);
    free(que);
}

// Finds the connected points where magic can flow
void floodFill(int start_row, int start_col) {
    Queue* que = createQueue(rows * columns);
    Point start = {start_row, start_col};
    enqueue(que, start);

    neighbor[start_row][start_col] = neighbor_count;
    neighbor_size[neighbor_count] = 1;

    while (!isEmpty(que)) {
        Point current = dequeue(que);
        int currentRow = current.row;
        int currentColumn = current.col;

        // Checks up, down, left, right
        for (int i = 0; i < 4; i++) {
            int newRow = currentRow + adjacentRow[i];
            int newColumn = currentColumn + adjacentColumn[i];

            // Checks if adjacent cell is a free point or not
            if (newRow >= 0 && newRow < rows && newColumn >= 0 && newColumn < columns &&
                grid[newRow][newColumn] == '.' && neighbor[newRow][newColumn] == -1) {
                neighbor[newRow][newColumn] = neighbor_count;
                neighbor_size[neighbor_count]++;
                Point next = {newRow, newColumn};
                enqueue(que, next);
            }
        }
    }

    neighbor_count++;
    freeQueue(que);
}

// Checks the pairs in a group of points
long long calculatePairs(int n) {
    return (long long) n * (n + 1) / 2;
}

// Finds max magic that can be generated by one failure
long long findMaxMagic() {
    long long max_magic = 0;
    long long current_magic = 0;

    // Calculates the starting magic
    for (int i = 0; i < neighbor_count; i++) {
        current_magic += calculatePairs(neighbor_size[i]);
    }

    // Checks each failure point
    for (int currentRow = 0; currentRow < rows; currentRow++) {
        for (int currentColumn = 0; currentColumn < columns; currentColumn++) {
            if (grid[currentRow][currentColumn] == 'X') {
                long long new_magic = current_magic;
                int neighboring_area[4] = {0};
                int neighbor_count = 0;
                int total_size = 1;

                // Checks the neighboring points
                for (int i = 0; i < 4; i++) {
                    int newRow = currentRow + adjacentRow[i];
                    int newColumn = currentColumn + adjacentColumn[i];
                    if (newRow >= 0 && newRow < rows && newColumn >= 0 && newColumn < columns && grid[newRow][newColumn] == '.') {
                        int reg = neighbor[newRow][newColumn];
                        int new_neighbor = 1;
                        for (int j = 0; j < neighbor_count; j++) {
                            if (neighboring_area[j] == reg) {
                                new_neighbor = 0;
                                break;
                            }
                        }
                        if (new_neighbor) {
                            neighboring_area[neighbor_count++] = reg;
                            new_magic -= calculatePairs(neighbor_size[reg]);
                            total_size += neighbor_size[reg];
                        }
                    }
                }

                new_magic += calculatePairs(total_size);

                if (new_magic > max_magic) {
                    max_magic = new_magic;
                }
            }
        }
    }

    return max_magic;
}
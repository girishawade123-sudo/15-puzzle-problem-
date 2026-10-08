#include <stdio.h>
#include <string.h>

#define N 4
#define MAX_NODES 100000

typedef struct {
    int board[N][N];
    int g, f;
    int parent;
    int dir;
    int done;
} Node;

Node nodes[MAX_NODES];
int count = 0;

int goal[N][N] = {
    {1,  2,  3,  4},
    {5,  6,  7,  8},
    {9,  10, 11, 12},
    {13, 14, 15, 0}
};

int dr[] = {-1, 1, 0, 0};
int dc[] = { 0, 0, -1, 1};
char *names[] = {"up", "down", "left", "right"};

int is_valid(int b[N][N]) {
    int seen[16] = {0};
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (b[i][j] < 0 || b[i][j] > 15 || seen[b[i][j]])
                return 0;
            seen[b[i][j]] = 1;
        }
    return 1;
}

int is_solvable(int b[N][N]) {
    int flat[16], k = 0, inversions = 0, blank_row = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] == 0)
                blank_row = i;
            else
                flat[k++] = b[i][j];

    for (int i = 0; i < k; i++)
        for (int j = i + 1; j < k; j++)
            if (flat[i] > flat[j])
                inversions++;

    int blank_from_bottom = N - blank_row;
    return (inversions + blank_from_bottom) % 2 == 1;
}

int misplaced(int b[N][N]) {
    int c = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] != 0 && b[i][j] != goal[i][j])
                c++;
    return c;
}

int already_seen(int b[N][N]) {
    for (int i = 0; i < count; i++)
        if (memcmp(nodes[i].board, b, sizeof(nodes[i].board)) == 0)
            return 1;
    return 0;
}

void add_node(int b[N][N], int g, int parent, int dir) {
    memcpy(nodes[count].board, b, sizeof(nodes[count].board));
    nodes[count].g = g;
    nodes[count].f = g + misplaced(b);
    nodes[count].parent = parent;
    nodes[count].dir = dir;
    count++;
}

void print_board(int b[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%2d ", b[i][j]);
        printf("\n");
    }
    printf("\n");
}

void print_path(int i, int show) {
    if (nodes[i].parent == -1) {
        if (show) {
            printf("Start:\n");
            print_board(nodes[i].board);
        }
        return;
    }
    print_path(nodes[i].parent, show);
    if (show) {
        printf("Move: %s\n", names[nodes[i].dir]);
        print_board(nodes[i].board);
    } else {
        printf("%s ", names[nodes[i].dir]);
    }
}

int main() {
    int start[N][N];

    printf("Enter the 16 tiles (0 = blank), row by row:\n");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &start[i][j]);

    if (!is_valid(start)) {
        printf("Invalid input: tiles 0-15 must each appear exactly once.\n");
        return 0;
    }
    if (!is_solvable(start)) {
        printf("Status: UNSOLVABLE.\n");
        return 0;
    }
    printf("Status: SOLVABLE.\n");

    int show;
    printf("Show intermediate boards? (1 = Yes, 0 = No): ");
    scanf("%d", &show);

    add_node(start, 0, -1, -1);
    int expanded = 0, goal_index = -1;

    while (1) {
        int cur = -1;
        for (int i = 0; i < count; i++)
            if (!nodes[i].done && (cur == -1 || nodes[i].f < nodes[cur].f))
                cur = i;
        if (cur == -1)
            break;

        nodes[cur].done = 1;
        expanded++;

        if (misplaced(nodes[cur].board) == 0) {
            goal_index = cur;
            break;
        }

        int br = 0, bc = 0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (nodes[cur].board[i][j] == 0) { br = i; bc = j; }

        for (int k = 0; k < 4; k++) {
            int nr = br + dr[k], nc = bc + dc[k];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N)
                continue;

            int b[N][N];
            memcpy(b, nodes[cur].board, sizeof(b));
            b[br][bc] = b[nr][nc];
            b[nr][nc] = 0;

            if (!already_seen(b) && count < MAX_NODES)
                add_node(b, nodes[cur].g + 1, cur, k);
        }
    }

    if (goal_index == -1) {
        printf("No solution found within the node limit.\n");
        return 0;
    }

    printf("\nSolution cost (moves): %d\n", nodes[goal_index].g);
    printf("States expanded: %d\n\n", expanded);

    if (show) {
        print_path(goal_index, 1);
    } else {
        printf("Moves: ");
        print_path(goal_index, 0);
        printf("\n");
    }
    return 0;
}

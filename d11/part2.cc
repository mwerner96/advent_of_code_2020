#include <iostream>
#include <fstream>
#include <stdexcept>

constexpr static int HEIGHT = 90;
constexpr static int WIDTH = 91;

inline bool on_board(int x, int y) {
    if (x < 0 || x > HEIGHT + 1 || y < 0 || y > WIDTH+1)
        return false;
    return true;
}

int count_adjacent(char board[HEIGHT+2][WIDTH+3], int x_in, int y_in) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int x = x_in;
            int y = y_in;
            if (i == 0 && j == 0)
                continue;
            do {
                x += i;
                y += j;
                if (board[x][y] == '.')
                    continue;
                if (board[x][y] == '#')
                    count++;
                break;
            } while(on_board(x, y));
        }
    }
    return count;
}

bool update_boards(char oldboard[HEIGHT+2][WIDTH+3], char newboard[HEIGHT+2][WIDTH+3]) {
    bool updated = false;
    for (int i = 1; i < HEIGHT+1; i++) {
        for (int j = 1; j < WIDTH+1; j++) {
            char prev = newboard[i][j];
            switch (oldboard[i][j]) {
            case '.':
                newboard[i][j] = '.';
                break;
            case 'L':
                newboard[i][j] = count_adjacent(oldboard, i, j) == 0 ? '#' : 'L';
                break;
            case '#':
                newboard[i][j] = count_adjacent(oldboard, i, j) >= 5 ? 'L' : '#';
                break;
            default:
                throw std::invalid_argument("Invalid Character.");
            }
            if (newboard[i][j] != prev)
                updated = true;
        }
    }
    return updated;
}

int main() {
    char board_1[HEIGHT+2][WIDTH+3]; // frame of floor-pieces (.) so we don't have edge-cases
    char board_2[HEIGHT+2][WIDTH+3] = {0};
    std::ifstream is("input");
    if (!is.is_open())
        exit(1);
    for (int i = 1; is.getline(&(board_1[i][1]), WIDTH+1) && (i <= HEIGHT); i++)
        ;
    is.close();
    for (int i = 0; i < HEIGHT + 2; i++) {
        board_1[i][0] = '.';
        board_1[i][WIDTH+1] = '.';
        board_1[i][WIDTH+2] = '\0';
        board_2[i][0] = '.';
        board_2[i][WIDTH+1] = '.';
        board_2[i][WIDTH+2] = '\0';
    }
    for (int i = 0; i < WIDTH + 2; i++) {
        board_1[0][i] = '.';
        board_1[HEIGHT + 1][i] = '.';
        board_2[0][i] = '.';
        board_2[HEIGHT + 1][i] = '.';
    }
    while (update_boards(board_1, board_2)) {
        update_boards(board_2, board_1);
    }
    int count = 0;
    for (int i = 1; i < HEIGHT+1; i++) {
        for (int j = 1; j < WIDTH+1; j++) {
            count += board_1[i][j] == '#' ? 1 : 0;
        }
    }
    std::cout << count << std::endl;
}

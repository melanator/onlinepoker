#include "../include/tictactoe.h"

void print_field(char *field){
    for (int i = 2; i >= 0; i--){
        for (int j = 0; j < 3; j++){
            std::cout << field[i*3 + j];
            if (j == 2){
                std::cout << "\n";
            }
            else {
                std::cout << " | ";
            }
        }
        if (i > 0){
            std::cout << "---------\n";
        }
    }
}

void fill_tile(char *field, char mark){
    int index;
    std::cout << "Select tile (1-9): ";
    std::cin >> index;
    field[index - 1] = mark;
}

int main(){
    std::cout << "Hello, world!" << std::endl;
    char field[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    print_field(field);
    for (int i = 0; i < 9; i++){
        fill_tile(field, 'x');
        print_field(field);
    }
    return 0;
}
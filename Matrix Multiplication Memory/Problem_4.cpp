#include <iostream>
#include <sstream>
#include <cstdlib>

void output(int** M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << M[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void zero(int** M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] = 0;
        }
    }
}

int main(int argc, char* args[]) {
    if (argc != 3) {
        std::cerr << "Error! Include an argument;" << std::endl;
        return -1;
    }

    const unsigned long n = atoi(args[1]);
    const unsigned long m = atoi(args[2]);
    int** M = new int*[n];
    for (int i = 0; i < n; i++) {
        M[i] = new int[n];
    }
    zero(M, n);
    int i, j, x;
    std::string input;
    std::srand(0); // warning biased
    for (int k = 0; k < m; k++) {
        i = std::rand() % n;
        j = std::rand() % n;
        x = 1 + std::rand() % 9;
        if (x % 2 == 0) x *= -1;
        M[i][j] += x;
    }
    output(M, n);
}

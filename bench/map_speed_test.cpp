#include <stdio.h>
#include <iostream>
#include <map>
#include <stdint.h>
#include <time.h>

#define SIZE 20

int main() {
    std::map<int, int> m;

    for(int i = 0; i < 1<<SIZE; i ++) {
        m[i] = i;
    }

    int r = 0;
    for(int j = 0; j < 1<<8; j ++) {
        for(int i = 0; i < 1<<SIZE; i ++) {
            r ^= m[i];
        }
    }

    for(int i = 0; i < 1<<SIZE; i ++) {
        m.erase(m.find(i));
    }

    return 0;
}

#include <stdlib.h>
#include <stdio.h>

int correct_midpoint(int x, int y){
    return (x + y)/2;
}

int midpoint(int x, int y) {
    if(x < 0 && y < 0) {
        return -midpoint(-x, -y);
    }

    if(x < 0 || y < 0) {
        return (x + y) / 2;
    }

    return (x / 2) + (y / 2) + (x % 2 + y % 2) / 2;
}
int main() {
    int x, y;

    while(scanf("%d %d", &x, &y) == 2){
        if(correct_midpoint(x, y) != midpoint(x, y)){
            printf("%d and %d\n", x, y);
            printf("should be %d\n", correct_midpoint(x, y));
            printf("but is %d\n\n", midpoint(x, y));
        }
    }
}
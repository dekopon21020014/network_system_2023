#include <stdio.h>

struct hoge {
    int  x;
    char y;
};

struct foo {
    char a[4];
    char b;
};

int main () {
	struct hoge h1;
    
    h1.x = 0x0f;
    h1.y = 'a';

    struct foo* f1;
    f1 = (struct foo *)&h1;
    
    printf ("f1.a[0] = %x, f1,b = %c\n", f1 -> a[0], f1 -> b);
    return 0;
}

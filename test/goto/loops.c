
#define ENABLE_WHILE 1
#define ENABLE_FOR 1
#define ENABLE_DO 1

#define ENABLE_WHILE_FOR 1
#define ENABLE_WHILE_DO 1
#define ENABLE_FOR_DO 1

#include <assert.h>

int main() {
#if ENABLE_WHILE
    {
        int x = 2;
        while(x) { --x; }
    }

    {
        int x = 2;
        while(x) { --x; break; }
    }

    {
        int x = 2;
        int z = 2;
        while(--x) { continue; z = 0; }
        assert(z != 0);
    }

    {
        int x = 2;
        while(--x) {
            int z = 0;
            switch(z) {
            case 0: {continue;} break;
            case 1: break;
            };
        }
    }
#endif

#if ENABLE_FOR
    {
        int i = 0;
        int x = 4;
        for (i = 0; i < 5; ++i) {
            --x;
        }
    }

    {
        int i = 0;
        int x = 4;
        for (i = 0; i < 5; ++i) {
            continue;
            --x;
        }
        assert(x == 4);
    }

   {
        int i = 0;
        int x = 4;
        for(; i < 5; ++i) {
            --x;
        }
    }
    {
        int i = 0;
        int x = 4;
        for(; i < 5; ) {
            --x;
            ++i;
        }
    }
    {
        int i = 0;
        int x = 4;
        for(; ; ) {
            if (i >= 5) break;
            --x;
            ++i;
        }
    }
    {
       int i = 0;
       for (i = 0; i < 5; ++i) {
           switch(i) {
           case 1: {continue;}break;
           case 2: {++i;}break;
           }
       }
    }
#endif

}


#define ENABLE_WHILE 1
#define ENABLE_FOR 1
#define ENABLE_DO 1

#define ENABLE_WHILE_FOR 1
#define ENABLE_WHILE_DO 1
#define ENABLE_FOR_DO 1

#define ENABLE_WHILE_FOR_DO 1

#include <assert.h>

#define CASE()

#define BLOCK(name)
#define ENDBLOC(name)


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
        for (i = 0; i < 5; ) {
            ++i;
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

#if ENABLE_DO
    {
        int i = 0;
        do {
            ++i;
        }while(i < 5);
    }
    {
        int i = 0;
        do {
            ++i;
            switch(i) {
            case 1: break;
            case 2:continue;
            }
        }while(i < 5);
    }

     {
        int i = 0;
        do {
            ++i;
            switch(i) {
            case 1: break;
            case 2:continue;
            }
        }while(i < 5);

     }

    {
        int k = 2;
        do {
            switch (k) {
            case 3:

                break;
            default:
                ++k;
                continue;
            }
            break;
            ++k;
        }while(k < 4);
    }
#endif

#if ENABLE_WHILE_FOR
    {
        int x = 0;
        int i = 0;
        while (++i < 3) {
            int j = 1;
            for (; j < 4; ++j) {
                continue;
                ++x;
            }
            continue;
            ++x;
        }
    }
    {
        int x = 0;
        int i = 0;
        while (++i < 3) {
            int j = 1;
            for (; j < 4; ++j, --x) {
                break;
                ++x;
            }
            break;
            ++x;
        }
    }

    {
        int x = 0;
        int i = 0;
        for (i = 2; i > 0; --i) {
            break;
            int x = 2;
            while (i > 0) {
                --x;
                switch(x) {
                case 1:break;
                case 0: continue; break;
                }
                break;
            }
            continue;
            ++x;
        }
    }
#endif


#if ENABLE_WHILE_DO
    {
        int i = 0;
        while (i++ < 10) {
            do {
                continue;
                break;
            }while(++i < 5);
            continue;
        }
    }

    {
        int i = 0;
        do {
            while(++i < 5) {
                continue;
                break;
            }
            continue;
            break;
        }while(++i < 10);
    }
#endif

#if ENABLE_FOR_DO

    {
        int i = 4;
        for (; i > 0; --i) {
            do {
                switch(i) {
                case 1: break;
                }
                --i;
                if (i > 10)
                    break;
            } while(i > 1);
            continue;
            if (i > 10)
                break;
        }
    }

    {
        int i = 5;
        do {
            if (i == 10)
                break;
            if (i == 2)
                continue;
            for (int j = 3; j <6; ++j ) {
                if (j == 5)
                    continue;
                if(j == 10)
                    break;
            }
            continue;
        }while(--i > 0);
    }
#endif
}


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
    {
    main_while_loop_begin_1:;
      if (!x)
        goto main_while_loop_end_1;
      { --x; }
      goto main_while_loop_begin_1;
    main_while_loop_end_1:;
    }
  }

  {
    int x = 2;
    {
    main_while_loop_begin_2:;
      if (!x)
        goto main_while_loop_end_2;
      {
        --x;
        goto main_while_loop_end_2;
      }
      goto main_while_loop_begin_2;
    main_while_loop_end_2:;
    }
  }

  {
    int x = 2;
    int z = 2;
    {
    main_while_loop_begin_3:;
      if (!--x)
        goto main_while_loop_end_3;
      {
        goto main_while_loop_begin_3;
        z = 0;
      }
      goto main_while_loop_begin_3;
    main_while_loop_end_3:;
    }

    assert(z != 0);
  }

  {
    int x = 2;
    {
    main_while_loop_begin_4:;
      if (!--x)
        goto main_while_loop_end_4;
      {
        int z = 0;
        switch (z) {
        case 0: {
          goto main_while_loop_begin_4;
          break;
        case 1:
          break;
        };
        }
      }
      goto main_while_loop_begin_4;
    main_while_loop_end_4:;
    }

#endif

#if ENABLE_FOR
    {
      int i = 0;
      int x = 4;
      {
        i = 0;
      main_for_loop_begin_1:;
        if (!(i < 5))
          goto main_for_loop_end_1;
        { --x; }
      main_for_loop_inc_1:;
        ++i;
        goto main_for_loop_begin_1;
      main_for_loop_end_1:;
      }
    }

    {
      int i = 0;
      int x = 4;
      {
        i = 0;
      main_for_loop_begin_2:;
        if (!(i < 5))
          goto main_for_loop_end_2;
        {
          ++i;
          goto main_for_loop_inc_2;
          --x;
        }
      main_for_loop_inc_2:;
        goto main_for_loop_begin_2;
      main_for_loop_end_2:;
      }

      assert(x == 4);
    }

    {
      int i = 0;
      int x = 4;
      {
      main_for_loop_begin_3:;
        if (!(i < 5))
          goto main_for_loop_end_3;
        { --x; }
      main_for_loop_inc_3:;
        ++i;
        goto main_for_loop_begin_3;
      main_for_loop_end_3:;
      }
    }
    {
      int i = 0;
      int x = 4;
      {
      main_for_loop_begin_4:;
        if (!(i < 5))
          goto main_for_loop_end_4;
        {
          --x;
          ++i;
        }
      main_for_loop_inc_4:;
        goto main_for_loop_begin_4;
      main_for_loop_end_4:;
      }
    }
    {
      int i = 0;
      int x = 4;
      {
      main_for_loop_begin_5:;
        {
          if (i >= 5)
            goto main_for_loop_end_5;
          --x;
          ++i;
        }
      main_for_loop_inc_5:;
        goto main_for_loop_begin_5;
      main_for_loop_end_5:;
      }
    }
    {
      int i = 0;
      {
        i = 0;
      main_for_loop_begin_6:;
        if (!(i < 5))
          goto main_for_loop_end_6;
        {
          switch (i) {
          case 1: {
            goto main_for_loop_inc_6;
            break;
          case 2: {
            ++i;
          } break;
          }
          }
        }
      main_for_loop_inc_6:;
        ++i;
        goto main_for_loop_begin_6;
      main_for_loop_end_6:;
      }

#endif

#if ENABLE_DO
      {
        int i = 0;
        {
        main_do_loop_begin_1:;
          { ++i; }
        main_do_loop_cond_1:;
          if (i < 5)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }
      {
        int i = 0;
        {
        main_do_loop_begin_1:;
          {
            ++i;
            switch (i) {
            case 1:
              break;
            case 2:
              goto main_do_loop_cond_2;
            }
          }
        main_do_loop_cond_1:;
          if (i < 5)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }

      {
        int i = 0;
        {
        main_do_loop_begin_1:;
          {
            ++i;
            switch (i) {
            case 1:
              break;
            case 2:
              goto main_do_loop_cond_3;
            }
          }
        main_do_loop_cond_1:;
          if (i < 5)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }

      {
        int k = 2;
        {
        main_do_loop_begin_1:;
          {
            switch (k) {
            case 3:
              break;
            default:
              ++k;
              goto main_do_loop_cond_4;
            }
            goto main_do_loop_end_4;
            ++k;
          }
        main_do_loop_cond_1:;
          if (k < 4)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }
#endif

#if ENABLE_WHILE_FOR
      {
        int x = 0;
        int i = 0;
        {
        main_while_loop_begin_5:;
          if (!(++i < 3))
            goto main_while_loop_end_5;
          {
            int j = 1;
            {
            main_for_loop_begin_7:;
              if (!(j < 4))
                goto main_for_loop_end_7;
              {
                goto main_for_loop_inc_7;
                ++x;
              }
            main_for_loop_inc_7:;
              ++j;
              goto main_for_loop_begin_7;
            main_for_loop_end_7:;
            }

            goto main_while_loop_begin_5;
            ++x;
          }
          goto main_while_loop_begin_5;
        main_while_loop_end_5:;
        }
      }
      {
        int x = 0;
        int i = 0;
        {
        main_while_loop_begin_6:;
          if (!(++i < 3))
            goto main_while_loop_end_6;
          {
            int j = 1;
            {
            main_for_loop_begin_8:;
              if (!(j < 4))
                goto main_for_loop_end_8;
              {
                goto main_for_loop_end_8;
                ++x;
              }
            main_for_loop_inc_8:;
              ++j, --x;
              goto main_for_loop_begin_8;
            main_for_loop_end_8:;
            }

            goto main_while_loop_end_6;
            ++x;
          }
          goto main_while_loop_begin_6;
        main_while_loop_end_6:;
        }
      }

      {
        int x = 0;
        int i = 0;
        {
          i = 2;
        main_for_loop_begin_9:;
          if (!(i > 0))
            goto main_for_loop_end_9;
          {
            goto main_for_loop_end_9;
            int x = 2;
            {
            main_while_loop_begin_7:;
              if (!(i > 0))
                goto main_while_loop_end_7;
              {
                --x;
                switch (x) {
                case 1:
                  break;
                case 0:
                  goto main_while_loop_begin_7;
                  break;
                }
                goto main_while_loop_end_7;
              }
              goto main_while_loop_begin_7;
            main_while_loop_end_7:;
            }
            goto main_for_loop_inc_9;
            ++x;
          }
        main_for_loop_inc_9:;
          --i;
          goto main_for_loop_begin_9;
        main_for_loop_end_9:;
        }
      }
#endif

#if ENABLE_WHILE_DO
      {
        int i = 0;
        {
        main_while_loop_begin_8:;
          if (!(i++ < 10))
            goto main_while_loop_end_8;
          {
            {
            main_do_loop_begin_1:;
              {
                goto main_do_loop_cond_5;
                goto main_do_loop_end_5;
              }
            main_do_loop_cond_1:;
              if (++i < 5)
                goto main_do_loop_begin_1;
            main_do_loop_end_1:;
            }

            goto main_while_loop_begin_8;
          }
          goto main_while_loop_begin_8;
        main_while_loop_end_8:;
        }
      }

      {
        int i = 0;
        {
        main_do_loop_begin_1:;
          {
            {
            main_while_loop_begin_9:;
              if (!(++i < 5))
                goto main_while_loop_end_9;
              {
                goto main_while_loop_begin_9;
                goto main_while_loop_end_9;
              }
              goto main_while_loop_begin_9;
            main_while_loop_end_9:;
            }
            goto main_do_loop_cond_6;
            goto main_do_loop_end_6;
          }
        main_do_loop_cond_1:;
          if (++i < 10)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }
#endif

#if ENABLE_FOR_DO

      {
        int i = 4;
        {
        main_for_loop_begin_10:;
          if (!(i > 0))
            goto main_for_loop_end_10;
          {
            {
            main_do_loop_begin_1:;
              {
                switch (i) {
                case 1:
                  break;
                }
                --i;
                if (i > 10)
                  goto main_do_loop_end_7;
              }
            main_do_loop_cond_1:;
              if (i > 1)
                goto main_do_loop_begin_1;
            main_do_loop_end_1:;
            }

            goto main_for_loop_inc_10;
            if (i > 10)
              goto main_for_loop_end_10;
          }
        main_for_loop_inc_10:;
          --i;
          goto main_for_loop_begin_10;
        main_for_loop_end_10:;
        }
      }

      {
        int i = 5;
        {
        main_do_loop_begin_1:;
          {
            if (i == 10)
              goto main_do_loop_end_8;
            if (i == 2)
              goto main_do_loop_cond_8;
            {
              int j = 3;
            main_for_loop_begin_11:;
              if (!(j < 6))
                goto main_for_loop_end_11;
              {
                if (j == 5)
                  goto main_for_loop_inc_11;
                if (j == 10)
                  goto main_for_loop_end_11;
              }
            main_for_loop_inc_11:;
              ++j;
              goto main_for_loop_begin_11;
            main_for_loop_end_11:;
            }
            goto main_do_loop_cond_8;
          }
        main_do_loop_cond_1:;
          if (--i > 0)
            goto main_do_loop_begin_1;
        main_do_loop_end_1:;
        }
      }
#endif
    }

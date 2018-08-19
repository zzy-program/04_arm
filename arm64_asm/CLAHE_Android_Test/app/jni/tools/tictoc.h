#ifndef TICTOC_H
#define TICTOC_H
#include <time.h>
class timer{
public:
    char name[128];
    clock_t start;
    clock_t stop;
    float elapse_ms;
    timer(char* _name);
    ~timer();
};
void tic( timer* t );
float toc(timer* t);
void print_timer(timer *_t, const char* user_info);

extern timer global_timer;
#endif /* TICTOC_H */

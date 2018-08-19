#include"tictoc.h"
#include<string.h>
#include"log1.h"
timer global_timer("global_timer");
#define LOG_TAG __FILE__
#define TIMER_BEG timer* t =&global_timer;      \
    if ( _t!= NULL) {                             \
        t = _t;                                \
    }                                           

timer::timer( char* _name ){
    strcpy(name, _name);
}
timer::~timer()
{
    
}
void tic( timer *_t)
{
    TIMER_BEG;
    
    t->start = clock();
} 

float toc( timer *_t)
{
    TIMER_BEG;
    t->stop = clock();
    t->elapse_ms = ((float)t->stop - t->start) * 1000 /CLOCKS_PER_SEC;
    return t->elapse_ms;
}

void print_timer(timer *_t, const char* user_info){
    TIMER_BEG;    
    LOG_D(LOG_TAG, "%s time %s's elapse is: %.4f", user_info, t->name, t->elapse_ms);
}
    

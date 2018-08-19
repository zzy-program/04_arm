#include "Histogram.h"
int main(int argc, char** argv)
{
    Histogram h(256);
    Histogram b;
    int i;
    for (i = 0; i < 3; i++) {

        h.pCounts[i]=i*100;
        
    }

    b.combine(&h, 1);

    Histogram chist;

    b.clip(0.5);
    b.cdf(&chist);
        
}

#ifndef HISTORGRAM_H
#define HISTORGRAM_H
#include <string.h>
#include "QImage.h"
class Histogram
{
public:
    int nBin;
    unsigned short* pCounts;

    Histogram(int _nBin = NBIN);
    ~Histogram();
    void clone( Histogram* pHist );
    void combine( Histogram* pHist, float fCombFactor);
    void cdf(Histogram* pHist);
    void collect(QImage* Im);
    void clip(float fContrastLimit);
    void print(); 
    void reset();
    int total();
};

#endif /* HISTORGRAM_H */

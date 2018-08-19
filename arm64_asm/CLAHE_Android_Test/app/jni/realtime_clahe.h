#ifndef REALTIME_CLAHE_H
#define REALTIME_CLAHE_H
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "Constants.h"
#include "Histogram.h"
#include "QImage.h"

class Clahe
{
public:
    int nTileX;
    int nTileY;
    Histogram* pHistArray;
    Histogram* cHist;
    UInt16* toneMapCache;
    Clahe( int _nTileX, int _nTileY );
    ~Clahe();
    int updateTileToneMapping( QImage* pIm , float fDampFactor); 
    int applyTileToneMapping( QImage* pIm, QImage* oIm, float fContrastLimit );
    void reset();
    void bilnearInterpolation( QImage* pIm, QImage* oIm );
    
};


#endif /* REALTIME_CLAHE_H */

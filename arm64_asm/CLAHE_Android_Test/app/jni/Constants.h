#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <assert.h>
#include "log1.h"
#include "tictoc.h"
typedef  unsigned char UInt8;
typedef  unsigned short UInt16;
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

#define NBIN 256

#define getPixel(Im, x, y) (( Im->pBuf + y*Im->nStride + x )[0])
#define getToneMap(MapCache, x, y, ntilex, nbin) (( MapCache[ (y* ntilex+x)*nbin] )[0])

#endif /* CONSTANTS_H */

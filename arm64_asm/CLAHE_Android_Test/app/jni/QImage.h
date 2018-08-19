#ifndef QIMAGE_H
#define QIMAGE_H
#include "Constants.h"
class QImage
{
public:
    UInt8* pBuf;
    int nWidth;
    int nHeight;
    int nStride;
    QImage();
    QImage( void* buf, int _nWidth, int _nHeight, int _nStride);
    ~QImage();
    int write(const char* filename);
    int read(const char* filename);

};


#endif /* QIMAGE_H */

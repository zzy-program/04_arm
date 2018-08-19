#include "QImage.h"
#include <stdlib.h>
#include "logcat.h"
QImage::QImage(){
    
}

QImage::QImage( void* buf, int _nWidth, int _nHeight, int _nStride){
    pBuf = (UInt8*)buf;
    nWidth = _nWidth;
    nHeight = _nHeight;
    nStride = _nStride;
}

QImage::~QImage(){
    
}

int QImage::write(const char* filename){
    FILE* f = fopen(filename,"w");
    int i,j;
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {

            fprintf(f,"%d",getPixel(this, j,i));
            if( j != nWidth -1)
                fprintf(f," ,");
        }
        fprintf(f,"\n");
    }

    fflush(f);
    int n=ftell(f);
    fclose(f);
    return n;
    
}

int QImage::read(const char* filename){
    FILE* f = fopen(filename,"r");
    int i,j;
    int n=fread(pBuf, sizeof(UInt8), nHeight*nWidth, f);
    fclose(f);
    return n;
    
}

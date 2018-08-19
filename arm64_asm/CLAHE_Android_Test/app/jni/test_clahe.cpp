#include "realtime_clahe.h"
#include "logcat.h"
int main_fun( )
{
    LOGI("%d",__LINE__);
    Clahe ahe(8,8);
    srand(0);
    int nWidth = 512, nHeight=512;
    
    UInt8* imBuf = new UInt8[nWidth * nHeight];
    int i;
    for (i = 0; i < nWidth*nHeight; i++) {
        imBuf[i] = rand();        
    }
    LOGI("%d",__LINE__);
    QImage Im, oIm;
    Im.pBuf = imBuf;
    Im.nWidth = nWidth;
    Im.nHeight = nHeight;
    Im.nStride = nWidth;
LOGI("%d",__LINE__);
    Im.read("/sdcard/clahe/lenna.txt");
    oIm  = Im;
    oIm.pBuf = new UInt8[nWidth * nHeight];
LOGI("%d",__LINE__);
    timer mytimer("totaltimer");
    tic(&mytimer);
    for (i = 0; i < 1000; i++) {
        
    ahe.updateTileToneMapping(&Im, 1.0);
    ahe.applyTileToneMapping(&Im, &oIm, 0.01); 

    }
LOGI("%d",__LINE__);
    toc(&mytimer);
    print_timer(&mytimer, __func__);
LOGI("%d",__LINE__);
    Im.write("/sdcard/clahe/im.txt");
LOGI("%d",__LINE__);
    oIm.write("/sdcard/clahe/oim.txt");
LOGI("%d",__LINE__);
    delete Im.pBuf;
    delete oIm.pBuf;
            
}

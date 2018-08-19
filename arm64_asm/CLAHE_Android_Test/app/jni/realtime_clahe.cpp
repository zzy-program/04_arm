#include <string.h>
#include "realtime_clahe.h"
//#include <tmmintrin.h>
//#include <intrin.h> 


#define LOG_TAG __FILE__
Clahe::Clahe(int _nTileX, int _nTileY)
{
    nTileX = _nTileX;
    nTileY = _nTileY;
    pHistArray = new Histogram[nTileX * nTileY];
    cHist = new Histogram[ nTileX * nTileY];
    toneMapCache = new UInt16[nTileX* nTileY *NBIN];
    memset(toneMapCache, 0, sizeof(UInt16) *nTileX *nTileY *NBIN);

}

Clahe::~Clahe()
{
    delete []pHistArray;
    delete []cHist;
    delete []toneMapCache;
}
void Clahe::reset()
{
    int i,j;
    for (i = 0; i < nTileX; i++) {
        for (j = 0; j < nTileY; j++) {
            pHistArray[j*nTileX +i].reset();
            cHist[j*nTileX +i].reset();
        }
        
    }
    memset(toneMapCache, 0, sizeof(UInt16) *nTileX *nTileY *NBIN);
}

 

   
int Clahe::updateTileToneMapping(QImage* pIm,  float fDampFactor){

    int nTileWidth, nTileHeight;

    nTileWidth = pIm->nWidth / nTileX;
    nTileHeight = pIm->nHeight / nTileY;
    //LOG_D(LOG_TAG, "Area of tile = %d", nTileWidth*nTileHeight);
    Histogram hist(NBIN);

    QImage Im;
    tic(NULL);

    Im.nWidth = nTileWidth;
    Im.nHeight = nTileHeight;
    Im.nStride = pIm->nWidth;
    int i,j;
    for (i = 0; i < nTileX; i++) {
        for (j = 0; j< nTileY; j++) {
            
            Im.pBuf = &getPixel( pIm, i*nTileWidth, j*nTileHeight);
           
            hist.collect(&Im);
            //LOG_D(LOG_TAG, "hist total = %d", hist.total());
            pHistArray[j*nTileX+i].combine( &hist, fDampFactor );
            //hist.print();
            hist.reset();
        }
    }
    toc(NULL);
    //print_timer(NULL, __func__);
    
 }


int Clahe::applyTileToneMapping( QImage* pIm, QImage* oIm, float fContrastLimit ){


    int nTileWidth, nTileHeight;
    int nTileArea;
    unsigned int invTileArea;
    unsigned int one =1;
    nTileWidth = pIm->nWidth / nTileX;
    nTileHeight = pIm->nHeight / nTileY;

    nTileArea =  nTileWidth * nTileHeight;

    invTileArea = ((one<<31)/nTileArea) >>16;
    // LOG_D(LOG_TAG, "nTileArea = %d, invTileArea %d = %.8f = %.8f",nTileArea,  invTileArea, invTileArea*1.0/pow(2,15) , 1.0/ nTileArea);
    QImage Im;
    Histogram hist(NBIN);

    
    
    UInt16* toneMap;
    int i,j,k;
    tic(NULL);
    
    Im.nWidth = nTileWidth;
    Im.nHeight = nTileHeight;
    Im.nStride = pIm->nWidth;

    for (i = 0; i < nTileX; i++) {
        for (j = 0; j< nTileY; j++) {
            Im.pBuf = &getPixel( pIm, i*nTileX, j*nTileY);

            hist.clone(&pHistArray[j*nTileX+i]);
            
            hist.clip( fContrastLimit );
            
            //Obtain Tone Mapping Function from CDF
            hist.cdf( &cHist[ j*nTileX +i] );
            
            toneMap =  &toneMapCache[ ( j * nTileX+ i )*NBIN ];

            //TODO:: lazy update of tone mapping function
            UInt16* pCounts = cHist[j*nTileX +i].pCounts;
            /**
             * Debug info print
             */ 
            //cHist[j*nTileX +i].print();
            
            for (k = 0; k < NBIN; k++) {
                //LOG_D(LOG_TAG, "%d %d ", pCounts[k], pCounts[NBIN-1]);
                toneMap[k] = (NBIN-1)*pCounts[k]/pCounts[NBIN-1];
                
            }
            
        }
    }

    toc(NULL);
    //print_timer(NULL, __func__);

    bilnearInterpolation( pIm, oIm);
    
}


inline void interpOpt( UInt8* y, UInt8* oy, UInt16* UL, UInt16* UR,UInt16* LL,UInt16* LR,int nX , int nY, int nW, int nH , int invTileArea,  int idx){


    UInt8 intensity = y[idx];
    UInt8 u[4];
    u[0] = UL[intensity];
    u[1] = UR[intensity];
    u[2] = LL[intensity];
    u[3] = LR[intensity];


            
    int w1 = (nH - nY);

    // aa = _mm_set_epi32(xTileRes, xTileRes, w1, w1);
    // bb = _mm_set_epi32(u[0], u[2], u[1], u[3]);

    // aa = _mm_mul_epi32(aa, bb);  //y1 * width 0 y2 *width 0
    // bb= _mm_shuffle_epi32(aa, _MM_SHUFFLE(2, 3, 0, 1));
    // bb = _mm_add_epi32(aa,bb);
    // f1 = bb[0];
    // f2= bb[1];
    int f1 = ( nY * u[0] + w1 * u[2] );
    int f2 = ( nY * u[1] + w1 * u[3] );

    //LOG_D(LOG_TAG, "%d " , u[1]);

    // f1 =  MulLUT[UL[intensity]][xTileRes] + MulLUT[UR[intensity]][nTileWidth - xTileRes];
    // f2 =  MulLUT[LL[intensity]][xTileRes] + MulLUT[LR[intensity]][nTileWidth - xTileRes];


    oy[idx] = ( (nX * f1 + (nW - nX) * f2) \
                         * invTileArea ) >>15;
        

}

void Clahe::bilnearInterpolation( QImage* pIm, QImage* oIm )
{
    int nTileWidth, nTileHeight;
    int nTileArea;
    unsigned int invTileArea;
    int nInvTileWidth;
    unsigned int one =1;
    nTileWidth = pIm->nWidth / nTileX;
    nTileHeight = pIm->nHeight / nTileY;

    nTileArea =  nTileWidth * nTileHeight;


    invTileArea = ((one<<31)/nTileArea) >>16;

    nInvTileWidth = ((one<<30)/nTileWidth) >>15;
 
    int i,j;
    int  xTileL,xTileH,yTileL,yTileH;
    int  xTileRes, yTileRes;
    UInt16* UL,*UR,*LL,*LR;

    // static int MulLUT[256][256];
    // int nLUTLen = max(nTileWidth , nTileHeight);

    // for (i = 0; i < nLUTLen; i++) {
    //     for (j = 0; j < NBIN; j++) {
    //         MulLUT[i][j] = i*j;
    //     }
    // }

    
    tic(NULL);
    int cnt=0;
    int dy0,dy1;

    UInt8 intensity;
    int f1,f2;    
    int a0,a1;
    int w0,w1;
    int u[4];
    //__m128i aa,bb;
    for (j = 0; j < pIm->nHeight; j++) {

        yTileL = (j - nTileHeight/2) / nTileHeight;
        yTileH = (j + nTileHeight/2) / nTileHeight;

        yTileRes = nTileHeight - (j + nTileHeight/2) % nTileHeight;

        dy0 = yTileL *nTileX;
        dy1 = min(nTileY-1, yTileH)  *nTileX;

        for (i = 0; i < pIm->nWidth; i+=16) {

            xTileL = max(0, (i- nTileWidth>>1) * nInvTileWidth >>15);//(i- nTileWidth>>1)/ nTileWidth;
            xTileH = max(0, (i+ nTileWidth>>1) * nInvTileWidth >>15);//(i+ nTileWidth>>1)/ nTileWidth;

            //LOG_D(LOG_TAG, "%d %d", xTileL,  max(0, (i- nTileWidth>>1) * nInvTileWidth >>15));
            xTileRes = nTileWidth - (i + nTileWidth>>1) % nTileWidth;
        

            UL = &toneMapCache[ (dy0 + xTileL )<<8 ];
            UR = &toneMapCache[ (dy0 + min( nTileX-1, xTileH) )<<8 ];
            LL = &toneMapCache[ ( dy1 +  xTileL) <<8 ];
            LR = &toneMapCache[ ( dy1 + min( nTileX-1, xTileH) )<<8 ];

            
            //Debug info print
            // LOG_D(LOG_TAG, "location:i=%d j=%d %d %d %d %d", i, j, max(0, xTileL), max(0, yTileL), min(nTileX-1, xTileH), yTileH);
            // LOG_D(LOG_TAG, "tonemap: %d %d %d %d", (max( 0, yTile -1)*nTileX+ max( 0, xTile -1))*NBIN, \
            //       (max( 0, yTile -1)*nTileX+ xTile )*NBIN,              \
            //       (yTile * nTileX+ max( 0, xTile -1))*NBIN,             \
            //       (  yTile * nTileX+ xTile )*NBIN);

            
            //bilinear interpolation

            intensity= pIm->pBuf[cnt];// getPixel( pIm, i, j );
            // u[0] = UL[intensity];
            // u[1] = UR[intensity];
            // u[2] = LL[intensity];
            // u[3] = LR[intensity];
            
            
            // w1 = (nTileWidth - xTileRes);

            // // aa = _mm_set_epi32(xTileRes, xTileRes, w1, w1);
            // // bb = _mm_set_epi32(u[0], u[2], u[1], u[3]);

            // // aa = _mm_mul_epi32(aa, bb);  //y1 * width 0 y2 *width 0
            // // bb= _mm_shuffle_epi32(aa, _MM_SHUFFLE(2, 3, 0, 1));
            // // bb = _mm_add_epi32(aa,bb);
            // // f1 = bb[0];
            // // f2= bb[1];
            // f1 = ( xTileRes * u[0] +  w1 * u[1] );
            // f2 = ( xTileRes * u[2] + w1 * u[3] );

            // // f1 =  MulLUT[UL[intensity]][xTileRes] + MulLUT[UR[intensity]][nTileWidth - xTileRes];
            // // f2 =  MulLUT[LL[intensity]][xTileRes] + MulLUT[LR[intensity]][nTileWidth - xTileRes];
            
            // oIm->pBuf[cnt++] = ( (yTileRes * f1 + (nTileHeight - yTileRes) * f2) \
            //                           * invTileArea ) >>15;

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 0);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 1);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 2);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 3);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 4);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 5);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 6);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 7);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 8);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 9);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 10);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 11);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 12);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 13);

            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 14);
            interpOpt(&pIm->pBuf[cnt], &oIm->pBuf[cnt], UL,UR, LL, LR, xTileRes, yTileRes, nTileWidth, nTileHeight, invTileArea, 15);

            cnt+=16;

                
        }
        
    }

    toc(NULL);
    //print_timer(NULL, __func__);
   
}


#include <math.h>
#include "Constants.h"
#include "Histogram.h"
#define LOG_TAG __FILE__

#ifdef QDEBUG
#define CHECK {LOG_D(LOG_TAG, __func__);print();}while(0)
#else
#define CHECK {}
#endif

Histogram::Histogram(int _nBin){
    nBin = _nBin;
    pCounts = new unsigned short[nBin];
    reset();
    
}

Histogram::~Histogram(){
    delete pCounts;
}

void Histogram::clone( Histogram* pHist){
    
    memcpy(pCounts, pHist->pCounts, sizeof(short)*nBin);
    CHECK;
}

void Histogram::combine( Histogram* pHist, float fCombFactor){
    int i;
    assert(fCombFactor >=0 && fCombFactor <=1);

    // unsigned int q15CombFactor = ((unsigned int)(fCombFactor* (1U<<31)))>>16;
    // unsigned int q15CombFactorR = (1<<15) - q15CombFactor;

    // for (i = 0; i < nBin; i+=4) {

    //     pCounts[i]= (pHist->pCounts[i]*q15CombFactor + pCounts[i]*q15CombFactorR)>>15;
    //     pCounts[i+1]= (pHist->pCounts[i+1]*q15CombFactor + pCounts[i+1]*q15CombFactorR)>>15;
    //     pCounts[i+2]= (pHist->pCounts[i+2]*q15CombFactor + pCounts[i+2]*q15CombFactorR)>>15;
    //     pCounts[i+3]= (pHist->pCounts[i+3]*q15CombFactor + pCounts[i+3]*q15CombFactorR)>>15;

    // }

    // Float point implementation
    for (i = 0; i < nBin; i++) {
        pCounts[i]= (pHist->pCounts[i]*fCombFactor + pCounts[i]*(1-fCombFactor));
    }

    CHECK;

}

void Histogram::cdf( Histogram* pHist ){

    int i;

    pHist->pCounts[0] = pCounts[0];

    for (i = 1; i < nBin; i++) {

        pHist->pCounts[i] = pHist->pCounts[i-1] + pCounts[i];
    }
#ifdef QDEBUG
    pHist->print();
#endif
}

void Histogram::clip(  float fContrastLimit )
{
    CHECK;
    assert(fContrastLimit >=0 && fContrastLimit <=1);
    int nTotal=0;
    int nTotalExcess = 0;
    int i;

    nTotal = total();
    //LOG_D(LOG_TAG, "nTotal =  %d", nTotal);
    
    int nContrastLimit = nTotal * fContrastLimit;

    //LOG_D(LOG_TAG, "nContrastLimit =  %d", nContrastLimit);
    
    for (i = 0; i < nBin; i++) {
        
        if ( nContrastLimit < pCounts[i] ) {

            nTotalExcess += pCounts[i] - nContrastLimit;
            
        }
    }

    int avgIncNum = floor(nTotalExcess/nBin);
    int upperBound = nContrastLimit - avgIncNum;
    for (i = 0; i < nBin; i++) {


        if(pCounts[i] <= upperBound){
            nTotalExcess -= avgIncNum;
            pCounts[i] += avgIncNum;            
        
        }
        else if (pCounts[i] < nContrastLimit ) {
            nTotalExcess -= nContrastLimit - pCounts[i];
            pCounts[i] = nContrastLimit;            
        }
        else{
            pCounts[i] = nContrastLimit;         
        }

    }

    int k=0;
    int nStep;
    while (nTotalExcess >0) {
        //LOG_D(LOG_TAG, "nTotalExcess remains %d", nTotalExcess);
        nStep = max(nBin/nTotalExcess, 1);
        
        for (i = k; i < nBin; i+=nStep) {

            if (pCounts[i] < nContrastLimit ) {
                pCounts[i]++;
                nTotalExcess--;
            }
            
            if (nTotalExcess <= 0 ) {
                break;
            }
        }
        
    }
    CHECK;
}

void Histogram::collect( QImage* Im){
    
    int i,j;
    
    for (j = 0; j < Im->nHeight; j++) {

        for (i = 0; i < Im->nWidth; i+=4) {            
            pCounts[getPixel(Im, i, j)]++;
            pCounts[getPixel(Im, i+1, j)]++;
            pCounts[getPixel(Im, i+2, j)]++;
            pCounts[getPixel(Im, i+3, j)]++;

        }
        
    }
    CHECK;
}

void Histogram::print(){
    int i;
    char s[1024*10];
    int n =0;
    for (i = 0; i < nBin; i++) {
        n+=sprintf(s+n,"%d ", pCounts[i]);
    }
    LOG_D(LOG_TAG, "%s", s);
    //printf("%s\n",s);
}

void Histogram::reset(){
    memset(pCounts, 0, nBin*sizeof(short));
}

inline int Histogram::total(){
    int i;
    int total=0;
    for (i = 0; i < nBin; ++i) {
        total+=pCounts[i] ;
    }
    return total;
}

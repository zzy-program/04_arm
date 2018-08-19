//输入一副图片，输出264裸流，用于生成带logo的TS

#include "logcat.h"
#include "com_zayhu_video_codec_ads.h"
#include "stdio.h"
#include <android/bitmap.h>
int main_fun( );

/*JNIEXPORT jint JNICALL Java_com_zayhu_video_player_MediaWaterMark_nativeTirgger
 (JNIEnv *env, jobject obj, jlong context,
 jbyteArray h264buffer,jint h264bufferlen
 )
 {

    return 0;
 }
*/
JNIEXPORT jint JNICALL Java_com_zayhu_video_player_MediaWaterMark_nativeTirgger
 ( jlong context,
 jbyteArray h264buffer,jint h264bufferlen
 )
 {

    main_fun( );
    return 0;
 }

package com.zayhu.video.player;

import android.graphics.Bitmap;
import android.os.Environment;
import android.util.Log;


import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.text.SimpleDateFormat;
import java.util.GregorianCalendar;
import java.util.Locale;

/**
 * 输入BMP图片，生成指定长度的TS文件，用于水印或者广告
 */
@SuppressWarnings("JniMissingFunction")
public class MediaWaterMark  {

    private  static boolean sLoadSuccess = false;
    static {
        //loadLibrary("clahe");
        sLoadSuccess = false;
        try {
            Log.i("CLAHELOG" , "Load clahe so success");
            System.loadLibrary("clahe");
            sLoadSuccess = true;
        } catch (Throwable e) {
            Log.i("CLAHELOG" , e.toString());
        }
    }
    private static boolean DEBUG = false;

    private long mCodec = 0;


    public MediaWaterMark(){

    }
    public int fire(){
        return nativeTirgger(0,null,0);
    }


    public void release() {
        //LOG.logI("release codec");
    }

    private native int nativeTirgger(long watermark,byte[] h264buffer,int h264bufferlen);

}

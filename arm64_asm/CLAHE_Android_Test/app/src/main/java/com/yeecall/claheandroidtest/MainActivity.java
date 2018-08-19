package com.yeecall.claheandroidtest;

import android.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.yeecall.claheandroidtest.ndk.JniUtils;
import com.zayhu.video.player.MediaWaterMark;

public class MainActivity extends AppCompatActivity {

    Button br ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);
    tv.setText("CLAHE测试床");

        Button btn=(Button)findViewById(R.id.button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MediaWaterMark mark = new  MediaWaterMark();
                mark.fire();
                String str = JniUtils.getStringFormC();
                String ming = "13550110110";
                String encrypmi = JniUtils.encode(ming);
                String decrypmi = JniUtils.decode(encrypmi);
            AlertDialog.Builder builder= new AlertDialog.Builder(MainActivity.this);//MainActivity.this为当前环境
                builder.setIcon(android.R.drawable.ic_dialog_info);//提示图标
                builder.setTitle("title");//提示框标题
                builder.setMessage("msg"+decrypmi);//提示内容
                builder.setPositiveButton("确认", null);//确定按钮
                builder.create().show();
            }
        });
    }
}




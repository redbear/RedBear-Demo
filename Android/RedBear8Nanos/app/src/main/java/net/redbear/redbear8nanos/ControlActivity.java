package net.redbear.redbear8nanos;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import com.google.gson.Gson;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import static net.redbear.redbear8nanos.Common.*;

/**
 * Created by dong on 16/3/29.
 */
public class ControlActivity extends Activity implements Correspondent.CorrespondentDataCallback,SeekBar.OnSeekBarChangeListener,CompoundButton.OnCheckedChangeListener {

    private static final String INTENT_DATA_IP = "data_ip";
    private static final String INTENT_DATA_PORT = "data_port";
    private static final String TAG = "ControlActivity";

    public static Intent getControlActivityIntent(Context context,String ip,int port){
        Intent intent = new Intent(context,ControlActivity.class);
        intent.putExtra(INTENT_DATA_IP,ip);
        intent.putExtra(INTENT_DATA_PORT,port);
        intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        return intent;
    }

    private Gson gson;

    Correspondent correspondent;
    private String ip;
    private int port;

    ArrayList<RGBDevice> devices;
    private static int DEVICES_NUM = 8;
    private static int MAX_DEVICES_NUM = 8;
    DataPackage dataPackage;
    float[] huv = new float[3];

    ArrayList<View> nanos;
    ArrayList<Switch> nanosSwitch;
    ArrayList<SeekBar> nanosSeekBar;
    View nano1,nano2,nano3,nano4,nano5,nano6,nano7,nano8;
    Switch nano1Switch,nano2Switch,nano3Switch,nano4Switch,nano5Switch,nano6Switch,nano7Switch,nano8Switch;
    SeekBar nano1SeekBar,nano2SeekBar,nano3SeekBar,nano4SeekBar,nano5SeekBar,nano6SeekBar,nano7SeekBar,nano8SeekBar;

    TextView ip_port;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);

        gson = new Gson();
        dataPackage = new DataPackage();

        initView();

        devices = new ArrayList<>();
        for (int i=0;i<MAX_DEVICES_NUM;i++){
            RGBDevice d = new RGBDevice();
            d.setId(i);
            devices.add(d);
        }

        ip = getIntent().getStringExtra(INTENT_DATA_IP);
        port = getIntent().getIntExtra(INTENT_DATA_PORT,0);
        ip_port.setText("IP : "+ip+"    PORT : "+port);

        correspondent = new Correspondent();
        correspondent.connect(ip,port);
        correspondent.setCallback(this);
    }

    @Override
    public void onBackPressed() {
        correspondent.disconnect();
        finish();
        super.onBackPressed();
    }

    private void initView(){

        ip_port = (TextView) findViewById(R.id.ip_port);

        nanos = new ArrayList<>();
        nano1 = findViewById(R.id.nano1);
        nano2 = findViewById(R.id.nano2);
        nano3 = findViewById(R.id.nano3);
        nano4 = findViewById(R.id.nano4);
        nano5 = findViewById(R.id.nano5);
        nano6 = findViewById(R.id.nano6);
        nano7 = findViewById(R.id.nano7);
        nano8 = findViewById(R.id.nano8);
        nanos.add(nano1);
        nanos.add(nano2);
        nanos.add(nano3);
        nanos.add(nano4);
        nanos.add(nano5);
        nanos.add(nano6);
        nanos.add(nano7);
        nanos.add(nano8);

        nanosSwitch = new ArrayList<>();
        nano1Switch = (Switch) findViewById(R.id.nano1_switch);
        nano2Switch = (Switch) findViewById(R.id.nano2_switch);
        nano3Switch = (Switch) findViewById(R.id.nano3_switch);
        nano4Switch = (Switch) findViewById(R.id.nano4_switch);
        nano5Switch = (Switch) findViewById(R.id.nano5_switch);
        nano6Switch = (Switch) findViewById(R.id.nano6_switch);
        nano7Switch = (Switch) findViewById(R.id.nano7_switch);
        nano8Switch = (Switch) findViewById(R.id.nano8_switch);
        nano1Switch.setOnCheckedChangeListener(this);
        nano2Switch.setOnCheckedChangeListener(this);
        nano3Switch.setOnCheckedChangeListener(this);
        nano4Switch.setOnCheckedChangeListener(this);
        nano5Switch.setOnCheckedChangeListener(this);
        nano6Switch.setOnCheckedChangeListener(this);
        nano7Switch.setOnCheckedChangeListener(this);
        nano8Switch.setOnCheckedChangeListener(this);
        nano1Switch.setTag(0);
        nano2Switch.setTag(1);
        nano3Switch.setTag(2);
        nano4Switch.setTag(3);
        nano5Switch.setTag(4);
        nano6Switch.setTag(5);
        nano7Switch.setTag(6);
        nano8Switch.setTag(7);
        nanosSwitch.add(nano1Switch);
        nanosSwitch.add(nano2Switch);
        nanosSwitch.add(nano3Switch);
        nanosSwitch.add(nano4Switch);
        nanosSwitch.add(nano5Switch);
        nanosSwitch.add(nano6Switch);
        nanosSwitch.add(nano7Switch);
        nanosSwitch.add(nano8Switch);

        nanosSeekBar = new ArrayList<>();
        nano1SeekBar = (SeekBar) findViewById(R.id.nano1_seekBar);
        nano2SeekBar = (SeekBar) findViewById(R.id.nano2_seekBar);
        nano3SeekBar = (SeekBar) findViewById(R.id.nano3_seekBar);
        nano4SeekBar = (SeekBar) findViewById(R.id.nano4_seekBar);
        nano5SeekBar = (SeekBar) findViewById(R.id.nano5_seekBar);
        nano6SeekBar = (SeekBar) findViewById(R.id.nano6_seekBar);
        nano7SeekBar = (SeekBar) findViewById(R.id.nano7_seekBar);
        nano8SeekBar = (SeekBar) findViewById(R.id.nano8_seekBar);
        nano1SeekBar.setMax(360);
        nano2SeekBar.setMax(360);
        nano3SeekBar.setMax(360);
        nano4SeekBar.setMax(360);
        nano5SeekBar.setMax(360);
        nano6SeekBar.setMax(360);
        nano7SeekBar.setMax(360);
        nano8SeekBar.setMax(360);
        nano1SeekBar.setOnSeekBarChangeListener(this);
        nano2SeekBar.setOnSeekBarChangeListener(this);
        nano3SeekBar.setOnSeekBarChangeListener(this);
        nano4SeekBar.setOnSeekBarChangeListener(this);
        nano5SeekBar.setOnSeekBarChangeListener(this);
        nano6SeekBar.setOnSeekBarChangeListener(this);
        nano7SeekBar.setOnSeekBarChangeListener(this);
        nano8SeekBar.setOnSeekBarChangeListener(this);
        nano1SeekBar.setTag(0);
        nano2SeekBar.setTag(1);
        nano3SeekBar.setTag(2);
        nano4SeekBar.setTag(3);
        nano5SeekBar.setTag(4);
        nano6SeekBar.setTag(5);
        nano7SeekBar.setTag(6);
        nano8SeekBar.setTag(7);
        nanosSeekBar.add(nano1SeekBar);
        nanosSeekBar.add(nano2SeekBar);
        nanosSeekBar.add(nano3SeekBar);
        nanosSeekBar.add(nano4SeekBar);
        nanosSeekBar.add(nano5SeekBar);
        nanosSeekBar.add(nano6SeekBar);
        nanosSeekBar.add(nano7SeekBar);
        nanosSeekBar.add(nano8SeekBar);
    }

    private void getDeviceInitData(){
        if (DEVICES_NUM == 0)
            return;
        new Timer().schedule(new TimerTask() {
            int i = 0;
            @Override
            public void run() {
                if (i >= DEVICES_NUM){
                    cancel();
                    return;
                }
                correspondent.send(gson.toJson(dataPackage.pack(devices.get(i++),OPCODE_READ)));
            }
        },100,200);
    }

    private void maskView(int count){
        if (count >= MAX_DEVICES_NUM)
            return;
        for (int i = 0;i < MAX_DEVICES_NUM;i++){
            if (i >= count){
                nanos.get(i).setVisibility(View.INVISIBLE);
            }else {
                nanos.get(i).setVisibility(View.VISIBLE);
            }
        }
    }

    @Override
    public void onConnectSuccess() {
        Log.i(TAG,"onConnectSuccess");
        correspondent.send(dataPackage.getNanoCountJsonString(gson));
    }

    @Override
    public void onConnectFail() {
        Log.i(TAG,"onConnectFail");
    }

    @Override
    public void onDisconnect() {
        Log.i(TAG,"onDisconnect");
    }

    @Override
    public void onDataReceive(String d) {
        try {
            upDateData(gson.fromJson(d,DataPackage.class));
        }catch (Exception e){}
    }

    private void upDateData(final DataPackage data){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (data == null)
                    return;

                if (data.getId() == DEV_COUNT_FLG){
                    if (data.getNum() > MAX_DEVICES_NUM)
                        return;
                    DEVICES_NUM = data.getNum();
                    maskView(DEVICES_NUM);
                    getDeviceInitData();
                }else{
                    if (data.getId() >= MAX_DEVICES_NUM)
                        return;
                    RGBDevice device = devices.get(data.getId());
                    device.setR(data.getR());
                    device.setG(data.getG());
                    device.setB(data.getB());
                    int num = data.getId();

                    nanos.get(num).setBackgroundColor(device.getColor());
                    if (device.getColor() != Color.BLACK){
                        nanosSwitch.get(num).setChecked(true);
                    }else {
                        nanosSwitch.get(num).setChecked(false);
                    }

                    Color.colorToHSV(device.getColor(),huv);
                    nanosSeekBar.get(num).setProgress((int)huv[0]);
                }
            }
        });
    }

    private void upDateData(int nanoN,int progress){
        huv[0] = progress;
        huv[1] = 1;
        huv[2] = 1;
        nanos.get(nanoN).setBackgroundColor(Color.HSVToColor(huv));
        nanosSwitch.get(nanoN).setChecked(true);
    }

    private void upDateData(int nanoN){
        devices.get(nanoN).setHUVColor(nanosSeekBar.get(nanoN).getProgress(),1,1);
        nanos.get(nanoN).setBackgroundColor(devices.get(nanoN).getColor());
        nanosSwitch.get(nanoN).setChecked(true);
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        int progress = seekBar.getProgress();
        int num = (Integer) seekBar.getTag();
        RGBDevice d = devices.get(num);
        d.setHUVColor(progress,1,1);
        correspondent.send(gson.toJson(dataPackage.pack(d,OPCODE_WRITE)));
        upDateData(num,progress);
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if(!buttonView.isPressed())return;
        int num = (Integer) buttonView.getTag();
        if (isChecked){
            upDateData(num);
            correspondent.send(gson.toJson(dataPackage.pack(devices.get(num),OPCODE_WRITE)));
        }else {
            correspondent.send(dataPackage.packOFFString(gson,OPCODE_WRITE,num));
            nanos.get(num).setBackgroundColor(Color.BLACK);
        }
    }
}

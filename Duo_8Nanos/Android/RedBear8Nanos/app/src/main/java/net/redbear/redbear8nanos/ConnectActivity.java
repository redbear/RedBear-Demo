package net.redbear.redbear8nanos;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


/**
 * This class is the first activity of app,
 * it ask user to input IP and PORT to connect the DUO by TCP/IP.
 *
 */
public class ConnectActivity extends Activity implements View.OnClickListener{
    public static final String PACKAGE_NAME = "net.redbear.redbear8nanos";
    public static final String SAVE_IP = "net.redbear.redbear8nanos.ip";
    public static final String SAVE_PORT = "net.redbear.redbear8nanos.port";

    private EditText ipInput,portInput;
    private Button connect;

    private String ip;
    private int port;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect);

        ipInput = (EditText) findViewById(R.id.connect_ip);
        portInput = (EditText) findViewById(R.id.connect_port);
        getInfo();

        connect = (Button) findViewById(R.id.connect);
        connect.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (!verifyIPFormat(ipInput.getText().toString())){
            Toast.makeText(this,"Please input a correct IP",Toast.LENGTH_SHORT).show();
            return;
        }

        if (!verifyPortFormat(portInput.getText().toString())){
            Toast.makeText(this,"Please input a correct Port",Toast.LENGTH_SHORT).show();
            return;
        }

        saveInfo();
        gotoControlActivity();
    }

    private boolean verifyIPFormat(String ip){
        if (ip != null && ip.length() > 0){
            if (ip.split("\\.").length == 4){
                this.ip = ip;
                return true;
            }
        }
        return false;
    }

    private boolean verifyPortFormat(String port){
        if (port != null && port.length() > 0){
            try {
                this.port = new Integer(port).intValue();
                return true;
            }catch (NumberFormatException n){
                return false;
            }
        }
        return false;
    }

    private void gotoControlActivity(){
        startActivity(ControlActivity.getControlActivityIntent(this,ip,port));
    }

    private void saveInfo(){
        SharedPreferences.Editor editor = getSharedPreferences(PACKAGE_NAME, Context.MODE_PRIVATE).edit();
        editor.putString(SAVE_IP, ip);
        editor.putInt(SAVE_PORT,port);
        editor.commit();
    }

    private void getInfo(){
        SharedPreferences read = getSharedPreferences(PACKAGE_NAME, Context.MODE_PRIVATE);

        ip = read.getString(SAVE_IP,"192.168.0.0");  //default ip    192.168.0.0
        port = read.getInt(SAVE_PORT,8888);          //default port  8888

        ipInput.setText(ip);
        portInput.setText(port+"");
    }

}

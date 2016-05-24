package net.redbear.redbear8nanos;

import android.os.Handler;
import android.util.Log;

import com.google.gson.Gson;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 * This class is used to communicate to DUO with socket
 */
public class Correspondent {

    private Socket socket;

    private OutputStream outputStream;
    private InputStream inputStream;
    private String ip;
    private int port;
    private static final int CONNECT_TIMEOUT = 5000;
    private Thread connectThread;
    private Thread receiveDataThread;
    private boolean CONNECTING;

    private CorrespondentDataCallback callback;

    public void connect(String ip,int port){
        this.ip = ip;
        this.port = port;
        if (CONNECTING)
            return;
        CONNECTING = true;
        connectThread = new Thread(connectRunnable);
        connectThread.start();
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (CONNECTING){
                    disconnect();
                    if (callback != null)
                        callback.onConnectFail();
                }
            }
        },CONNECT_TIMEOUT);
    }

    private Runnable connectRunnable = new Runnable() {
        @Override
        public void run() {
            try {
                socket = new Socket(ip,port);
                CONNECTING = false;
                outputStream = socket.getOutputStream();
                inputStream = socket.getInputStream();
                if (callback != null)
                    callback.onConnectSuccess();
                receiveData();
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
        }
    };

    private void receiveData(){
        receiveDataThread = new Thread(receiveDataRunnable);
        receiveDataThread.start();
    }

    private Runnable receiveDataRunnable = new Runnable() {
        @Override
        public void run() {
            try {
                byte[] buffer = new byte[1000];
                int temp = -1;
                while((temp = inputStream.read(buffer)) != -1){
                    byte[] data = new byte[temp];
                    for(int i = 0; i < data.length;i++) {
                        data[i] = buffer[i];
                    }
                    String receiveData = new String(data);
                    if (callback != null && receiveData != null){
                        callback.onDataReceive(receiveData);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
        }
    };

    public void send(String data){
        Log.e("data",">>>>>  "+data);
        if (outputStream != null){
            try {
                outputStream.write(data.getBytes());
                outputStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void disconnect() {
        if (connectThread != null && connectThread.isAlive()){
            connectThread.interrupt();
        }
        if (receiveDataThread != null && receiveDataThread.isAlive()){
            receiveDataThread.interrupt();
        }
        try {
            if (outputStream != null)
                outputStream.close();
            if (inputStream != null)
                inputStream.close();
            if (socket != null)
                socket.close();
        }catch (Exception e){

        }
    }

    public void setCallback(CorrespondentDataCallback callback) {
        this.callback = callback;
    }

    public interface CorrespondentDataCallback {
        void onConnectSuccess();
        void onConnectFail();
        void onDisconnect();
        void onDataReceive(String d);
    }
}

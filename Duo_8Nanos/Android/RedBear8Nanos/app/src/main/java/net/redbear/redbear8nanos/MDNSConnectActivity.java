package net.redbear.redbear8nanos;

import android.app.Activity;
import android.content.Context;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdServiceInfo;
import android.os.Bundle;
import android.util.Log;

import java.net.InetAddress;

/**
 * Created by dong on 16/5/13.
 */
public class MDNSConnectActivity extends Activity{

    NsdManager mNsdManager;
    private NsdManager.DiscoveryListener discoveryListener;
    String TAG = "mdns";
    private static final String NDMS_SERVICE_TYPE = "_duosample._tcp.";

    private String ip;
    private int port;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mdns_connect);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mNsdManager = (NsdManager) getSystemService(Context.NSD_SERVICE);
        discoveryListener = new NsdManager.DiscoveryListener() {
            @Override
            public void onDiscoveryStarted(String regType) {
                Log.i(TAG, "Service discovery started........");
            }

            @Override
            public void onServiceFound(NsdServiceInfo service) {
                mNsdManager.resolveService(service,new NsdManager.ResolveListener() {
                    @Override
                    public void onResolveFailed(NsdServiceInfo serviceInfo, int errorCode) {
                    }

                    @Override
                    public void onServiceResolved(NsdServiceInfo serviceInfo) {
                        Log.i(TAG, "Resolve Succeeded. " + serviceInfo);
                        InetAddress host = serviceInfo.getHost();
                        port = serviceInfo.getPort();
                        ip = host.getHostAddress();
                        gotoControlActivity();
                    }
                });
            }

            @Override
            public void onServiceLost(NsdServiceInfo service) {
                Log.i(TAG, "service lost........" + service);
            }

            @Override
            public void onDiscoveryStopped(String serviceType) {
                Log.i(TAG, "Discovery stopped: .........." + serviceType);
            }

            @Override
            public void onStartDiscoveryFailed(String serviceType, int errorCode) {
                Log.i(TAG, "onStartDiscoveryFailed...........: Error code:" + errorCode);
                mNsdManager.stopServiceDiscovery(this);
            }

            @Override
            public void onStopDiscoveryFailed(String serviceType, int errorCode) {
                Log.i(TAG, "onStopDiscoveryFailed............: Error code:" + errorCode);
                mNsdManager.stopServiceDiscovery(this);
            }
        };
        startNDMS();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mNsdManager.stopServiceDiscovery(discoveryListener);
    }

    void startNDMS(){
        mNsdManager.discoverServices(NDMS_SERVICE_TYPE, NsdManager.PROTOCOL_DNS_SD,discoveryListener);
    }

    private void gotoControlActivity(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (ip == null)
                    return;
                startActivity(ControlActivity.getControlActivityIntent(MDNSConnectActivity.this,ip,port));
                finish();
            }
        });
    }
}

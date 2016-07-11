package com.gcea.androidmicrobittest;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();
    private static final int REQUEST_ENABLE_BT = 1;

    private TextView console;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        console = (TextView) findViewById(R.id.txtConsole);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.menuConnect:
                connectMicroBit();
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                if (resultCode == RESULT_CANCELED)
                    Toast.makeText(this, "Bluetooth was not enabled.", Toast.LENGTH_SHORT).show();
                else {
                    Toast.makeText(this, "Bluetooth enabled!", Toast.LENGTH_SHORT).show();
                    connectMicroBit();
                }
                break;

            default:
                super.onActivityResult(requestCode, resultCode, data);
        }
    }

    // Scan for and connect Bluetooth device
    private void connectMicroBit() {
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // Bail out if we can't find a BT adapter
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Your device does not support Bluetooth!", Toast.LENGTH_SHORT).show();
            return;
        }

        // If BT isn't enabled, ask user to enable
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            return;
        }

        // Get all paired BT devices on phone, dump them to log
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();

        if (pairedDevices.size() > 0)
            for (BluetoothDevice device : pairedDevices)
                console.append("Paired device found; name=\"" + device.getName() + "\", addr=\"" + device.getAddress() + "\"\n");
    }
}

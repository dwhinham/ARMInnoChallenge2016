package com.gcea.microbitencryptiondemo;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();
    private static final String ACTION_USB_PERMISSION = "com.gcea.androidmicrobittest.USB_PERMISSION";

    private TextView status;
    private TextView messages;

    private CheckBox chkEncrypt;
    private EditText edtMessage;
    private ImageButton btnSend;

    private UsbManager usbManager;
    private UsbDevice microBit;
    private UsbDeviceConnection microBitConnection;
    UsbSerialDevice serial;

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                Toast.makeText(MainActivity.this, "Permission granted!", Toast.LENGTH_LONG).show();
                connectMicroBit(intent);
            }
            if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                Toast.makeText(MainActivity.this, "micro:bit attached!", Toast.LENGTH_LONG).show();
                connectMicroBit(intent);
            }
            if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                Toast.makeText(MainActivity.this, "micro:bit detached!", Toast.LENGTH_LONG).show();
                disconnectMicroBit();
            }
        }
    };

    private UsbSerialInterface.UsbReadCallback mSerialReceiveCallback = new UsbSerialInterface.UsbReadCallback() {
        @Override
        public void onReceivedData(final byte[] bytes)
        {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    messages.append(new String(bytes));
                }
            });
        }
    };

    @Override
    protected void onPause() {
        super.onPause();

        this.unregisterReceiver(mUsbReceiver);
    }

    @Override
    protected void onResume() {
        super.onResume();

        IntentFilter filter = new IntentFilter();
        filter.addAction(ACTION_USB_PERMISSION);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);

        this.registerReceiver(mUsbReceiver, filter);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        status = (TextView) findViewById(R.id.txtStatus);
        messages = (TextView) findViewById(R.id.txtMessages);

        chkEncrypt = (CheckBox) findViewById(R.id.chkEncrypt);
        edtMessage = (EditText) findViewById(R.id.edtMessage);
        btnSend = (ImageButton) findViewById(R.id.btnSend);

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    String message = edtMessage.getText().toString();

                    if (message.length() == 0)
                        return;

                    if (serial == null)
                        return;

                    if (chkEncrypt.isChecked())
                        message = "!" + message;

                    message += '\r';

                    serial.write(message.getBytes());
                    edtMessage.setText("");
                } catch (Exception e) {
                    messages.append(e.getMessage());
                }
            }
        });

        Intent intent = getIntent();
        if (intent != null && intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED))
            connectMicroBit(intent);
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
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    // Open USB Serial on BBC micro:bit
    private void connectMicroBit(Intent intent) {
        microBit = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

        if (!usbManager.hasPermission(microBit))
        {
            PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
            usbManager.requestPermission(microBit, pendingIntent);
            return;
        }

        status.setText("Connected");
        status.setTextColor(Color.GREEN);

        microBitConnection = usbManager.openDevice(microBit);

        serial = UsbSerialDevice.createUsbSerialDevice(microBit, microBitConnection);
        serial.open();
        serial.setBaudRate(9600);
        serial.setDataBits(UsbSerialInterface.DATA_BITS_8);
        serial.setParity(UsbSerialInterface.PARITY_ODD);
        serial.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);

        serial.read(mSerialReceiveCallback);
    }

    private void disconnectMicroBit() {
        status.setText("Disconnected");
        status.setTextColor(Color.RED);

        if (serial != null)
            serial.close();
    }
}

package br.com.rafaelvalentim.ledsbluetoothandroid;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends Activity {
    private static final String TAG = "LEDOnOff";
    Button btnLedRed, btnLedYellow, btnLedGreen, btnLedWhite, btnDesligarTodos, btnLigarTodos;
    private static final int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;
    // Well known SPP UUID
    private static final UUID MY_UUID =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // Insert your bluetooth devices MAC address
    private static String address = "98:D3:31:F7:35:F5";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        verificaStatusBluetooth();
        fazerConexoesDoLayoutListeners();
    }
    @Override
    public void onPause() { super.onPause(); }
    private void verificaStatusBluetooth() {
        btAdapter = BluetoothAdapter.getDefaultAdapter();
// Check for Bluetooth support and then check to make sure it is turned on
// Emulator doesn't support Bluetooth and will return null
        if(btAdapter == null) {
            errorExit("Fatal Error", "Bluetooth Not supported. Aborting.");
        } else {
            if (!btAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(btAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch(requestCode){
            case REQUEST_ENABLE_BT:
                if(resultCode == Activity.RESULT_OK){
                    Toast.makeText(getApplicationContext(), "Bluetooth foi ativado", Toast.LENGTH_LONG).show();
                } else {
                    Toast.makeText(getApplicationContext(), "Bluetooth nao foi ativado", Toast.LENGTH_LONG).show();
                }
                break;
        }
    }
    public void fazerConexoesDoLayoutListeners() {

    }


    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG, "...In onResume - Attempting client connect...");
// Set up a pointer to the remote node using it's address.
        BluetoothDevice device = btAdapter.getRemoteDevice(address);
        try {
            btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (IOException e) {
            errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
        }
// Discovery is resource intensive. Make sure it isn't going on when you attempt to connect and pass your message.
        btAdapter.cancelDiscovery();
// Establish the connection. This will block until it connects.
        Log.d(TAG, "...Connecting to Remote...");
        try {
            btSocket.connect();
        } catch (IOException e) {
            try {
                btSocket.close();
            } catch (IOException e2) {
                errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
            }
        }
// Create a data stream so we can talk to server.
        try {
            outStream = btSocket.getOutputStream();
        } catch (IOException e) {
            errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");
        }
    }
    private void errorExit(String title, String message){
        Toast msg = Toast.makeText(getBaseContext(),
                title + " - " + message, Toast.LENGTH_SHORT);
        msg.show();
        finish();
    }
    private void sendData(String message) {
        byte[] msgBuffer = message.getBytes();
        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if (address.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address to the correct address in the java code";
            msg = msg + ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";
            errorExit("Fatal Error", msg);
        }
    }

    public void notaDo(View view) {
        sendData(String.valueOf(1));
    }

    public void notaRe(View view) {
        sendData(String.valueOf(2));
    }

    public void notaMi(View view) {
        sendData(String.valueOf(3));
    }

    public void notaFa(View view) {
        sendData(String.valueOf(4));
    }

    public void notaSol(View view) {
        sendData(String.valueOf(5));
    }

    public void notaLa(View view) {
        sendData(String.valueOf(6));
    }

    public void notaSi(View view) {
        sendData(String.valueOf(7));
    }
}
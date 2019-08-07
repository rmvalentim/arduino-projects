package br.com.rafaelvalentim.chatbluetooth;

import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Set;
import java.util.UUID;
public class BluetoothChatActivity extends AppCompatActivity
        implements View.OnClickListener, DialogInterface.OnCancelListener {
    private TextView status;
    private static final String SERVICO = "DominandoChat";
    private static final UUID MEU_UUID =
            UUID.fromString("2accaffd-18dd-43ac-a2c4-623550cf9c8f");
    private static final int BT_TEMPO_DESCOBERTA = 30;
    private static final int BT_ATIVAR = 0;
    private static final int BT_VISIVEL = 1;
    private static final int MSG_TEXTO = 0;
    private static final int MSG_DESCONECTOU = 2;
    private static final int MSG_CONECTOU = 3;
    private ThreadServidor mThreadServidor;
    private ThreadCliente mThreadCliente;
    private ThreadComunicacao mThreadComunicacao;
    private BluetoothAdapter mAdaptadorBluetooth;
    private ArrayAdapter<String> mDispositivosEncontrados;
    private EventosBluetoothReceiver mEventosBluetoothReceiver;
    private DataInputStream is;
    private DataOutputStream os;
    private ArrayAdapter<String> mMensagens;
    private TelaHandler mTelaHandler;
    private ListView lstMensagens;
    ProgressBar progressBar;
    private Dialog dialog;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_chat);

        status = findViewById(R.id.status);
        progressBar = findViewById(R.id.note_list_progress);
        mTelaHandler = new TelaHandler();
        mMensagens = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        lstMensagens =  findViewById(R.id.lstHistorico);
        lstMensagens.setAdapter(mMensagens);
        mEventosBluetoothReceiver = new EventosBluetoothReceiver();
        mDispositivosEncontrados = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        mAdaptadorBluetooth = BluetoothAdapter.getDefaultAdapter();
        if (mAdaptadorBluetooth != null) {
            if (!mAdaptadorBluetooth.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, BT_ATIVAR);
            }
        } else {
            Toast.makeText(this, R.string.msg_erro_bt_indisponivel, Toast.LENGTH_LONG).show();
            finish();
        }
        IntentFilter filter1 = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        IntentFilter filter2 = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(mEventosBluetoothReceiver, filter1);
        registerReceiver(mEventosBluetoothReceiver, filter2);
        findViewById(R.id.btnEnviar).setOnClickListener(this);
    }
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mEventosBluetoothReceiver);
        paraTudo();
    }
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_bluetooth_chat, menu);
        return super.onCreateOptionsMenu(menu);
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_cliente:
                mDispositivosEncontrados.clear();
                mAdaptadorBluetooth.startDiscovery();
                exibirProgressDialog(R.string.msg_procurando_dispositivos, 0);
                break;
            case R.id.action_servidor:
                Intent discoverableIntent = new Intent(
                        BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                discoverableIntent.putExtra(
                        BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION,
                        BT_TEMPO_DESCOBERTA);
                startActivityForResult(discoverableIntent, BT_VISIVEL);
                break;
        }
        return super.onOptionsItemSelected(item);
    }
    public void onCancel(DialogInterface dialog) {
        mAdaptadorBluetooth.cancelDiscovery();
        paraTudo();
    }
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == BT_ATIVAR) {
            if (RESULT_OK != resultCode) {
                Toast.makeText(this, R.string.msg_ativar_bluetooth, Toast.LENGTH_SHORT).show();
                finish();
            }
        } else if (requestCode == BT_VISIVEL) {
            if (resultCode == BT_TEMPO_DESCOBERTA) {
                iniciaThreadServidor();
            } else {
                Toast.makeText(this, R.string.msg_aparelho_invisivel, Toast.LENGTH_SHORT).show();
            }
        }
    }
    private void exibirDispositivosBluetooth() {
        dialog = new Dialog(this);
        dialog.setContentView(R.layout.layout_bluetooth);
        dialog.setTitle("Dispositivos Bluetooth");
//Inicializando os Adaptadores Bluetooth
        ArrayAdapter<String> pairedDevicesAdapter = new ArrayAdapter<>(this,
                android.R.layout.simple_list_item_1);
//Vinculando as listas de dispositivos com a UI
        ListView listView = (ListView) dialog.findViewById(R.id.pairedDeviceList);
        ListView listView2 = (ListView) dialog.findViewById(R.id.discoveredDeviceList);
        listView.setAdapter(pairedDevicesAdapter);
        listView2.setAdapter(mDispositivosEncontrados);
        Set<BluetoothDevice> pairedDevices = mAdaptadorBluetooth.getBondedDevices();
//Se existir dispositivos já pareados, adicionar a lista
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                pairedDevicesAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            pairedDevicesAdapter.add(getString(R.string.msg_none_paired));
        }
//Manipulando os eventos de clique das Listas de Dispositivos
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                mAdaptadorBluetooth.cancelDiscovery();
                String info = ((TextView) view).getText().toString();
                String address = info.substring(info.length() - 17);
                BluetoothDevice device = mAdaptadorBluetooth.getRemoteDevice(address);
                iniciaThreadCliente(device);
                dialog.dismiss();
            }
        });
        listView2.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                mAdaptadorBluetooth.cancelDiscovery();
                String info = ((TextView) view).getText().toString();
                String address = info.substring(info.length() - 17);
                BluetoothDevice device = mAdaptadorBluetooth.getRemoteDevice(address);
                iniciaThreadCliente(device);
                dialog.dismiss();
            }
        });
        dialog.findViewById(R.id.cancelButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        });
        dialog.setCancelable(false);
        dialog.show();
    }
    public void onClick(View v) {
        EditText edt = (EditText) findViewById(R.id.edtMsg);
        String msg = edt.getText().toString();
        edt.setText("");
        try {
            if (os != null) {
                os.writeUTF(msg); //Escreve uma string no OutputStream
                mMensagens.add("Eu: " + msg);
                mMensagens.notifyDataSetChanged(); //atualiza a lista que exibe as mensagens

            }
        } catch (IOException e) {
            e.printStackTrace();
            mTelaHandler.obtainMessage(MSG_DESCONECTOU, e.getMessage() + "[0]").sendToTarget();
        }
    }
    private void exibirProgressDialog(int mensagem, long tempo) {
        progressBar.setVisibility(View.VISIBLE);
        if (tempo > 0) {
            mTelaHandler.postDelayed(new Runnable() { //agenda a execução de uma ação
                public void run() { //após um determinado tempo
                    if (mThreadComunicacao == null) {
                        progressBar.setVisibility(View.INVISIBLE);
                    }
                }
            }, tempo * 1000);
        }
    }
    private void setStatus(String s) {
        status.setText(s);
    }
    private void paraTudo() {
        if (mThreadComunicacao != null) {
            mThreadComunicacao.parar();
            mThreadComunicacao = null;
        }
        if (mThreadServidor != null) {
            mThreadServidor.parar();
            mThreadServidor = null;
        }
        if (mThreadCliente != null) {
            mThreadCliente.parar();
            mThreadCliente = null;
        }
    }
    private void iniciaThreadServidor() {
        exibirProgressDialog(R.string.mensagem_servidor, BT_TEMPO_DESCOBERTA);
        paraTudo();
        mThreadServidor = new ThreadServidor();
        mThreadServidor.iniciar();
    }
    private void iniciaThreadCliente(BluetoothDevice device) {
        paraTudo();
        mThreadCliente = new ThreadCliente();
        mThreadCliente.iniciar(device);
    }
    private void trataSocket(final BluetoothSocket socket) {
        runOnUiThread(new Runnable() {

            @Override
            public void run() {

                progressBar.setVisibility(View.INVISIBLE);

            }
        });

        mThreadComunicacao = new ThreadComunicacao();
        mThreadComunicacao.iniciar(socket);
    }
    private class ThreadServidor extends Thread {
        BluetoothServerSocket serverSocket;
        BluetoothSocket clientSocket;
        public void run() {
            try {
                serverSocket = mAdaptadorBluetooth.
                        listenUsingRfcommWithServiceRecord(SERVICO, MEU_UUID);
                clientSocket = serverSocket.accept();
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {

                        mTelaHandler.obtainMessage(MSG_CONECTOU, "Conectado com: " +
                                clientSocket.getRemoteDevice().getName()).sendToTarget();

                    }
                });


                trataSocket(clientSocket);
            } catch (IOException e) {
                mTelaHandler.obtainMessage(MSG_DESCONECTOU, e.getMessage() + "[1]").sendToTarget();
                e.printStackTrace();
            }
        }
        public void iniciar() {
            start();
        }
        public void parar() {
            try {
                serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    private class ThreadCliente extends Thread {
        BluetoothDevice device;
        BluetoothSocket socket;
        public void run() {
            try {
                socket = device.createRfcommSocketToServiceRecord(MEU_UUID);
                socket.connect();
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {

                        mTelaHandler.obtainMessage(MSG_CONECTOU, "Conectado com: " +
                                device.getName()).sendToTarget();

                    }
                });


                trataSocket(socket);
            } catch (IOException e) {
                e.printStackTrace();
                mTelaHandler.obtainMessage(MSG_DESCONECTOU, e.getMessage() + "[2]").sendToTarget();
            }
        }
        public void iniciar(BluetoothDevice device) {
            this.device = device;
            start();
        }
        public void parar() {
            try {
                socket.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    private class ThreadComunicacao extends Thread {
        String nome;
        BluetoothSocket socket;
        public void run() {
            try {
                nome = socket.getRemoteDevice().getName(); //Busca o nome do dispositivo remoto
                is = new DataInputStream(socket.getInputStream());
                os = new DataOutputStream(socket.getOutputStream());


                String string;
                while (true) {
                    string = is.readUTF();


                    mTelaHandler.obtainMessage(MSG_TEXTO, nome + ": " + string).sendToTarget();
                    scrollListViewToBottom();
                }
            } catch (IOException e) {
                e.printStackTrace();
                mTelaHandler.obtainMessage(MSG_DESCONECTOU,
                        e.getMessage() + "[3]").sendToTarget();
            }
        }
        public void iniciar(BluetoothSocket socket) {
            this.socket = socket;
            start();
        }
        public void parar() {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    private class EventosBluetoothReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction())) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    mDispositivosEncontrados.add(device.getName() + "\n" + device.getAddress());
                }
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(intent.getAction())) {
                if (mDispositivosEncontrados.getCount() == 0) {
                    mDispositivosEncontrados.add(getString(R.string.msg_none_found));
                }
                exibirDispositivosBluetooth();
            }
        }
    }
    private class TelaHandler extends Handler {
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == MSG_TEXTO) {
                mMensagens.add(msg.obj.toString());
                mMensagens.notifyDataSetChanged();

            } else if(msg.what == MSG_CONECTOU) {
                setStatus(msg.obj.toString());
            } else if (msg.what == MSG_DESCONECTOU) {
                Toast.makeText(BluetoothChatActivity.this,
                        getString(R.string.msg_desconectou) + msg.obj.toString(),
                        Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void scrollListViewToBottom() {
        lstMensagens.post(new Runnable() {
            @Override
            public void run() {

                lstMensagens.setSelection(mMensagens.getCount() - 1);
            }
        });
    }
}
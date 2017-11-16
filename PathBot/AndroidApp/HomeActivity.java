package com.pathbotapp.pathbot;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.os.AsyncTask;



/**
 * Created by Nikhil Grover on 11/15/2017.
 */

public class HomeActivity extends Activity implements View.OnClickListener {

    public final static String PREF_IP = "PREF_IP_ADDRESS";
    public final static String PREF_PORT = "PREF_PORT_NUMBER";

    // buttons and text inputs
    private Button buttonPin11, buttonPin12, buttonPin13;
    private EditText editIP, editPort;

    SharedPreferences.Editor editor;
    SharedPreferences sharedPreferences;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        sharedPreferences = getSharedPreferences("HTTP_HELPER_PREFS", Context.MODE_PRIVATE);

        buttonPin11 = (Button)findViewById(R.id.buttonPin11);
        buttonPin12 = (Button)findViewById(R.id.buttonPin12);
        buttonPin13 = (Button)findViewById(R.id.buttonPin13);

        editIP = (EditText)findViewById(R.id.editIP);
        editPort = (EditText)findViewById(R.id.editPort);

        buttonPin11.setOnClickListener(this);
        buttonPin12.setOnClickListener(this);
        buttonPin13.setOnClickListener(this);

        editIP.setText(sharedPreferences.getString(PREF_IP, ""));
        editPort.setText(sharedPreferences.getString(PREF_PORT, ""));
    }

    public void onClick(View view) {
        String paramValue = "";
        String ip = editIP.getText().toString().trim();
        String port = editPort.getText().toString().trim();

        editor.putString(PREF_IP, ip);
        editor.putString(PREF_PORT, port);

        if(view.getId() == buttonPin11.getId()) {
            paramValue = "11";
        } else if(view.getId() == buttonPin12.getId()) {
            paramValue = "12";
        } else {
            paramValue = "13";
        }

        //send HTTP request
        if(ip.length() > 0 && port.length() > 0) {
            
        }
    }
}

package com.rookiedev.smarty

import android.app.Activity
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.InetAddresses.isNumericAddress
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.text.method.LinkMovementMethod
import android.widget.Button
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.google.android.material.tabs.TabLayout
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout


class MainActivity : AppCompatActivity() {
    companion object {
        private const val SHARED_PREFS_NAME = "com.rookiedev.smarty_preferences"
        private const val SHARED_PREFS_IP = "IP"
        private const val SHARED_PREFS_PORT = "PORT"

        private const val ERROR_NO_PERMISSION = 0
        private const val ERROR_NO_DEVICE = 1
        private const val ERROR_BLUETOOTH_DISABLED = 2
    }

    private var mContext: Context? = null

    private lateinit var ipInput: TextInputEditText
    private lateinit var portInput: TextInputEditText

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mContext = applicationContext

        // TCP
        val ipLayout = findViewById<TextInputLayout>(R.id.ip_input_layout)
        val portLayout = findViewById<TextInputLayout>(R.id.port_input_layout)
        ipInput = findViewById(R.id.ip_input)
        portInput = findViewById(R.id.port_input)

        // connect button
        val connectButton = findViewById<Button>(R.id.button_connect)

        // GitHub link
        val sourceLink = findViewById<TextView>(R.id.textView_github)
        sourceLink.movementMethod = LinkMovementMethod.getInstance()

        // read shared preference
        readSharedPref()

        connectButton.setOnClickListener {
            if (isNumericAddress(ipInput.text.toString()) && portInput.text.toString()
                    .toInt() >= 0 && portInput.text.toString().toInt() <= 65535
            ) {
                saveSharedPref()
                val intent = Intent(this, ControlActivity::class.java).apply {
                    putExtra("ip", ipInput.text.toString())
                    putExtra("port", portInput.text.toString())
                }
                startActivity(intent)
            } else if (!isNumericAddress(ipInput.text.toString())) {
                ipLayout.error = getString(R.string.invalid_ip)
            } else {
                portLayout.error = getString(R.string.invalid_port)
            }
        }


        ipInput.addTextChangedListener(object : TextWatcher {
            override fun onTextChanged(s: CharSequence, start: Int, before: Int, count: Int) {
                ipLayout.error = null
            }

            override fun beforeTextChanged(s: CharSequence, start: Int, count: Int, after: Int) {}
            override fun afterTextChanged(s: Editable) {}
        })

        portInput.addTextChangedListener(object : TextWatcher {
            override fun onTextChanged(s: CharSequence, start: Int, before: Int, count: Int) {
                portLayout.error = null
            }

            override fun beforeTextChanged(s: CharSequence, start: Int, count: Int, after: Int) {}
            override fun afterTextChanged(s: Editable) {}
        })
    }

    override fun onPause() {
        super.onPause()
        saveSharedPref()
    }

    private fun readSharedPref() {
        val prefs = getSharedPreferences(
            SHARED_PREFS_NAME,
            MODE_PRIVATE
        )
        // read values from the shared preferences
        ipInput.setText(prefs.getString(SHARED_PREFS_IP, "192.168.1.127"))
        portInput.setText(prefs.getString(SHARED_PREFS_PORT, "1234"))

    }

    private fun saveSharedPref() {
        val prefs = getSharedPreferences(
            SHARED_PREFS_NAME,
            MODE_PRIVATE
        )
        val editor = prefs.edit()
        editor.putString(SHARED_PREFS_IP, ipInput.text.toString())
        editor.putString(SHARED_PREFS_PORT, portInput.text.toString())

        editor.apply()
    }

    private fun alertDialog(type: Int) {
        val alert: AlertDialog = AlertDialog.Builder(this).create()
        when (type) {
            ERROR_NO_PERMISSION -> {
                alert.setTitle("Permission Error")
                alert.setIcon(R.drawable.ic_baseline_error_24)
                alert.setMessage(
                    "Bluetooth permission is required. Please enable the permission in Settings."
                )
                alert.setOnCancelListener { }
                alert.setButton(
                    AlertDialog.BUTTON_POSITIVE,
                    "OK"
                ) { _, _ -> }
            }
            ERROR_NO_DEVICE -> {
                alert.setTitle("Empty Device")
                alert.setIcon(R.drawable.ic_baseline_error_24)
                alert.setMessage(
                    "Please select a Bluetooth device."
                )
                alert.setOnCancelListener { }
                alert.setButton(
                    AlertDialog.BUTTON_POSITIVE,
                    "OK"
                ) { _, _ -> }
            }
            ERROR_BLUETOOTH_DISABLED -> {
                alert.setTitle("Bluetooth Disabled")
                alert.setIcon(R.drawable.ic_baseline_error_24)
                alert.setMessage(
                    "Please enable Bluetooth in Settings"
                )
                alert.setOnCancelListener { }
                alert.setButton(
                    AlertDialog.BUTTON_POSITIVE,
                    "OK"
                ) { _, _ -> }
            }
        }
        alert.show()
    }
}
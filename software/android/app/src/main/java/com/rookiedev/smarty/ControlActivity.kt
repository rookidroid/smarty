package com.rookiedev.smarty

import android.annotation.SuppressLint
import android.app.Activity
import android.app.AlertDialog
import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.*
import android.widget.SeekBar
import androidx.constraintlayout.widget.ConstraintLayout
import com.rookiedev.smarty.network.TCPClient
import com.rookiedev.smarty.view.VerticalSeekBar
import kotlinx.coroutines.*
import java.io.IOException
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import java.net.SocketException


/**
 * Behaviors of immersive mode.
 */
enum class BehaviorOption(
    val title: String,
    val value: Int
) {
    // Swipe from the edge to show a hidden bar. Gesture navigation works regardless of visibility
    // of the navigation bar.
    Default(
        "BEHAVIOR_DEFAULT",
        WindowInsetsController.BEHAVIOR_DEFAULT
    ),

    // "Sticky immersive mode". Swipe from the edge to temporarily reveal the hidden bar.
    ShowTransientBarsBySwipe(
        "BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE",
        WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
    )
}

/**
 * Type of system bars to hide or show.
 */
enum class TypeOption(
    val title: String,
    val value: Int
) {
    // Both the status bar and the navigation bar
    SystemBars(
        "systemBars()",
        WindowInsets.Type.systemBars()
    ),

    // The status bar only.
    StatusBar(
        "statusBars()",
        WindowInsets.Type.statusBars()
    ),

    // The navigation bar only
    NavigationBar(
        "navigationBars()",
        WindowInsets.Type.navigationBars()
    )
}

class ControlActivity : Activity() {
    private var mContext: Context? = null

    private var tcpClient: TCPClient? = null
    private var ip: String = ""
    private var port = 0

    private val scope = CoroutineScope(Job() + Dispatchers.IO)

    private lateinit var progressBar: ConstraintLayout

    private var seekBarLeft: VerticalSeekBar? = null
    private var seekBarRight: VerticalSeekBar? = null

    private var leftSpeed = 0
    private var rightSpeed = 0

    private var UDPSocket: DatagramSocket? = null


    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_control)

        val myIntent = intent // gets the previously created intent

        mContext = applicationContext

        ip = myIntent.getStringExtra("ip").toString()
        port = myIntent.getStringExtra("port").toString().toInt()

        seekBarLeft = findViewById(R.id.seekBar_l)
        seekBarRight = findViewById(R.id.seekBar_r)

        controlWindowInsets(true)

        progressBar = findViewById(R.id.progressBar)

        seekBarLeft!!.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar, i: Int, b: Boolean) {
                val msg = "L"
                val speed = ((i - 255)/10)*10
                if (leftSpeed!=speed) {
                    sendMessageAsync(msg.plus(speed.toString()).plus(":"))
                    leftSpeed=speed
                }
            }

            override fun onStartTrackingTouch(seekBar: SeekBar) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar) {
                seekBar.progress = 255
                sendMessageAsync("L0:")
            }
        })

        seekBarRight!!.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar, i: Int, b: Boolean) {
                val msg = "R"
                val speed = ((i - 255)/10)*10
                if (rightSpeed!=speed) {
                    sendMessageAsync(msg.plus(speed.toString()).plus(":"))
                    rightSpeed = speed
                }
            }

            override fun onStartTrackingTouch(seekBar: SeekBar) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar) {
                seekBar.progress = 255
                sendMessageAsync("R0:")
            }
        })

        try {
            UDPSocket = DatagramSocket()
        } catch (e1: SocketException) {
            // TODO Auto-generated catch block
            e1.printStackTrace()
        }
        try {
            UDPSocket!!.soTimeout = 3000 // set receive wait time
        } catch (e1: SocketException) {
            // TODO Auto-generated catch block
            e1.printStackTrace()
        }

    }

    override fun onResume() {
        super.onResume()
        progressBar.visibility = View.GONE

//        this.tcpClient = TCPClient(ip, port, object : TCPClient.OnMessageReceived {
//            override fun messageReceived(message: String?) {
//                if (message == null) {
//                    println("no message")
//                }
//            }
//        }, object : TCPClient.OnConnectEstablished {
//            override fun onConnected() {
//                Handler(Looper.getMainLooper()).post {
//                    progressBar.visibility = View.GONE
//                }
//            }
//        }, object : TCPClient.OnDisconnected {
//            override fun onDisconnected() {
//                Handler(Looper.getMainLooper()).post {
//                    progressBar.visibility = View.GONE
//                    alertDialog(0)
//                }
//            }
//        }
//        )
//        this.tcpClient!!.start()

        seekBarLeft!!.progress = 255
        seekBarRight!!.progress = 255
        leftSpeed = 0
        rightSpeed = 0
    }

    override fun onPause() {
        super.onPause()
//        tcpClient!!.stopClient()
//        tcpClient!!.interrupt()
    }


    private fun controlWindowInsets(hide: Boolean) {
        // WindowInsetsController can hide or show specified system bars.
        val insetsController = window.decorView.windowInsetsController ?: return
        // The behavior of the immersive mode.
        val behavior = BehaviorOption.values()[1].value
        // The type of system bars to hide or show.
        val type = TypeOption.values()[0].value
        insetsController.systemBarsBehavior = behavior
        if (hide) {
            insetsController.hide(type)
        } else {
            insetsController.show(type)
        }
    }

    fun sendMessageAsync(message: String) {
        // Starts a new coroutine within the scope
        scope.launch {
            // New coroutine that can call suspend functions
            withContext(Dispatchers.IO) {
//                tcpClient?.sendMessage(message)
                sendUdpMessage(message)
            }

        }
    }


    fun sendUdpMessage(UDPMessage: String){
        val bufferData = UDPMessage.toByteArray()
        val UDPOut = DatagramPacket(
            bufferData, bufferData.count(),
            InetAddress.getByName(ip), port
        )
        try {
            UDPSocket!!.send(UDPOut)
        } catch (e: IOException) {
            // TODO Auto-generated catch block
            e.printStackTrace()
        }
    }

    fun alertDialog(type: Int) {
        val alert: AlertDialog = AlertDialog.Builder(this).create()
        when (type) {
            0 -> {
                alert.setTitle("Error")
                alert.setIcon(R.drawable.ic_baseline_error_24)
                alert.setMessage(
                    "Unable to connect to the robot."
                )
                alert.setOnCancelListener { finish() }
                alert.setButton(
                    AlertDialog.BUTTON_POSITIVE,
                    "OK"
                ) { _, _ -> finish() }
            }
        }
        alert.show()
    }
}

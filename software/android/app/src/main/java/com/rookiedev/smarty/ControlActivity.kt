package com.rookiedev.smarty

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.view.WindowInsetsController
import android.widget.SeekBar
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import com.rookiedev.smarty.view.VerticalSeekBar
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.IOException
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import java.net.SocketException


/**
 * Behaviors of immersive mode.
 */
enum class BehaviorOption(
    val title: String, val value: Int
) {
    // Swipe from the edge to show a hidden bar. Gesture navigation works regardless of visibility
    // of the navigation bar.
    Default(
        "BEHAVIOR_DEFAULT", WindowInsetsController.BEHAVIOR_DEFAULT
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
    val title: String, val value: Int
) {
    // Both the status bar and the navigation bar
    SystemBars(
        "systemBars()", WindowInsets.Type.systemBars()
    ),

    // The status bar only.
    StatusBar(
        "statusBars()", WindowInsets.Type.statusBars()
    ),

    // The navigation bar only
    NavigationBar(
        "navigationBars()", WindowInsets.Type.navigationBars()
    )
}

class ControlActivity : AppCompatActivity() {
    private var mContext: Context? = null
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
        enableEdgeToEdge()
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
                val speed = ((i - 255) / 10) * 10
                if (leftSpeed != speed) {
                    sendMessageAsync(msg.plus(speed.toString()).plus(":"))
                    leftSpeed = speed
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
                val speed = ((i - 255) / 10) * 10
                if (rightSpeed != speed) {
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

        seekBarLeft!!.progress = 255
        seekBarRight!!.progress = 255
        leftSpeed = 0
        rightSpeed = 0
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

    private fun sendUdpMessage(UDPMessage: String) {
        val bufferData = UDPMessage.toByteArray()
        val udpOut = DatagramPacket(
            bufferData, bufferData.count(), InetAddress.getByName(ip), port
        )
        try {
            UDPSocket!!.send(udpOut)
        } catch (e: IOException) {
            // TODO Auto-generated catch block
            e.printStackTrace()
        }
    }
}

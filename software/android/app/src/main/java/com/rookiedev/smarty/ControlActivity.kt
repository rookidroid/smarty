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
import androidx.appcompat.widget.AppCompatSeekBar
import androidx.constraintlayout.widget.ConstraintLayout
import com.rookiedev.smarty.network.TCPClient
import kotlinx.coroutines.*


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
    companion object {
        private const val CMD_STANDBY = "standby:"
        private const val CMD_LAYDOWN = "laydown:"

        private const val CMD_WALK_0 = "walk0:"
        private const val CMD_WALK_180 = "walk180:"

        private const val CMD_WALK_R45 = "walkr45:"
        private const val CMD_WALK_R90 = "walkr90:"
        private const val CMD_WALK_R135 = "walkr135:"

        private const val CMD_WALK_L45 = "walkl45:"
        private const val CMD_WALK_L90 = "walkl90:"
        private const val CMD_WALK_L135 = "walkl135:"

        private const val CMD_FASTFORWARD = "fastforward:"
        private const val CMD_FASTBACKWARD = "fastbackward:"

        private const val CMD_TURNLEFT = "turnleft:"
        private const val CMD_TURNRIGHT = "turnright:"

        private const val CMD_CLIMBFORWARD = "climbforward:"
        private const val CMD_CLIMBBACKWARD = "climbbackward:"

        private const val CMD_ROTATEX = "rotatex:"
        private const val CMD_ROTATEY = "rotatey:"
        private const val CMD_ROTATEZ = "rotatez:"

        private const val CMD_TWIST = "twist:"
    }

    private var rightWidth = 0
    private var rightHeight = 0
    private var rightRadius = 0f

    private var leftWidth = 0
    private var leftHeight = 0

    private var connectInterface: String = ""

    private var mContext: Context? = null

    private var tcpClient: TCPClient? = null
    private var ip: String = ""
    private var port = 0

    private val scope = CoroutineScope(Job() + Dispatchers.IO)

    private var currentState: String = CMD_STANDBY
    private lateinit var progressBar: ConstraintLayout

    private var seekBarLeft: VerticalSeekBar? = null
    private var seekBarRight: VerticalSeekBar? = null


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
                // Display the current progress of SeekBar
//                println(i)
                val msg = "L"
                val speed = i - 255
                sendMessageAsync(msg.plus(speed.toString()).plus(":"))
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
                // Display the current progress of SeekBar
//                println(i)
                val msg = "R"
                val speed = i - 255
                sendMessageAsync(msg.plus(speed.toString()).plus(":"))
            }

            override fun onStartTrackingTouch(seekBar: SeekBar) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar) {
                seekBar.progress = 255
                sendMessageAsync("R0:")
            }
        })

    }

    @SuppressLint("MissingPermission")
    override fun onResume() {
        super.onResume()
        progressBar.visibility = View.VISIBLE

        this.tcpClient = TCPClient(ip, port, object : TCPClient.OnMessageReceived {
            override fun messageReceived(message: String?) {
                if (message == null) {
//                    alertDialog(DISCONNECTED)
                    println("no message")
                }
            }
        }, object : TCPClient.OnConnectEstablished {
            override fun onConnected() {
//                udpClient.start()
                println("connected")
                Handler(Looper.getMainLooper()).post {
                    progressBar.visibility = View.GONE
                }
            }
        }, object : TCPClient.OnDisconnected {
            override fun onDisconnected() {
                Handler(Looper.getMainLooper()).post {
                    progressBar.visibility = View.GONE
                    alertDialog(0)
                }
            }
        }
        )
        this.tcpClient!!.start()


        currentState = CMD_STANDBY
        seekBarLeft!!.progress = 255
        seekBarRight!!.progress = 255
    }

    override fun onPause() {
        super.onPause()
        tcpClient!!.stopClient()
        tcpClient!!.interrupt()
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

            withContext(Dispatchers.IO) {              // Dispatchers.IO (main-safety block)
                tcpClient?.sendMessage(message)
            }
        }
    }

    fun alertDialog(type: Int) {
        val alert: AlertDialog = AlertDialog.Builder(this).create()
        when (type) {
            0 -> {
                alert.setTitle("Error")
                alert.setIcon(R.drawable.ic_baseline_error_24)
                alert.setMessage(
                    "Unable to connect to the Hexapod."
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


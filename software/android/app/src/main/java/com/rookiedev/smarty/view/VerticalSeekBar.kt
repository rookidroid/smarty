package com.rookiedev.smarty.view

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Canvas
import android.util.AttributeSet
import android.view.MotionEvent
import android.widget.SeekBar


@SuppressLint("AppCompatCustomView")
open class VerticalSeekBar : SeekBar {
    private var changeListener: OnSeekBarChangeListener? = null

    constructor(context: Context?) : super(context!!)
    constructor(context: Context?, attrs: AttributeSet?, defStyle: Int) : super(
        context!!,
        attrs,
        defStyle
    )

    constructor(context: Context?, attrs: AttributeSet?) : super(context!!, attrs)

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(h, w, oldh, oldw)
    }

    @Synchronized
    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(heightMeasureSpec, widthMeasureSpec)
        setMeasuredDimension(measuredHeight, measuredWidth)
    }

    override fun onDraw(c: Canvas) {
        c.rotate(-90f)
        c.translate((-height).toFloat(), 0f)
        super.onDraw(c)
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (!isEnabled) {
            return false
        }
        when (event.action) {
            MotionEvent.ACTION_MOVE -> {
                val mHeight = height - paddingStart - paddingEnd
                var i = max - (max * (event.y - paddingStart) / mHeight).toInt()
                if (i<0){
                    i=0
                }
                if (i>max){
                    i=max
                }
                progress = i
                changeListener?.onProgressChanged(this, progress, true)
                onSizeChanged(width, height, 0, 0)
            }
            MotionEvent.ACTION_DOWN -> {
                changeListener?.onStartTrackingTouch(this)
                onSizeChanged(width, height, 0, 0)
            }
            MotionEvent.ACTION_UP -> {
                changeListener?.onStopTrackingTouch(this)
                onSizeChanged(width, height, 0, 0)
            }
            MotionEvent.ACTION_CANCEL -> {}
        }
        return true
    }

    override fun setOnSeekBarChangeListener(mListener: OnSeekBarChangeListener) {
        this.changeListener = mListener
    }

    @Synchronized
    override fun setProgress(progress: Int) {
        super.setProgress(progress)
        onSizeChanged(width, height, 0, 0)
    }
}
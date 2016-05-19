package com.xiaying.gpapi;

/**
 * Created by jiangxiaotang on 16/5/19.
 */
public class AutoRecycle {
    public void recycle() {
    }

    @Override
    protected void finalize() {
        recycle();
    }
}

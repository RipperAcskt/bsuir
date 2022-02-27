package com.example.web.polinom;

public class Polinom {
    private int len;
    private boolean polinom;

    public Polinom(int len, boolean polinom) {
        this.len = len;
        this.polinom = polinom;
    }

    public int getLen() {
        return len;
    }

    public void setLen(int len) {
        this.len = len;
    }

    public boolean isPolinom() {
        return polinom;
    }

    public void setPolinom(boolean polinom) {
        this.polinom = polinom;
    }
}

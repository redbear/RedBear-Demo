package net.redbear.redbear8nanos;

import android.graphics.Color;

import com.google.gson.Gson;
import com.google.gson.annotations.SerializedName;

/**
 * Created by dong on 16/3/29.
 */
public class RGBDevice {
    @SerializedName("ID")
    private int id;
    @SerializedName("R")
    private int r = 255;
    @SerializedName("G")
    private int g;
    @SerializedName("B")
    private int b;


    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getR() {
        return r;
    }

    public void setR(int r) {
        this.r = r;
    }

    public int getG() {
        return g;
    }

    public void setG(int g) {
        this.g = g;
    }

    public int getB() {
        return b;
    }

    public void setB(int b) {
        this.b = b;
    }

    public int getColor(){
        return Color.rgb(r,g,b);
    }

    public void setRGBColor(int r,int g, int b){
        this.r = r;
        this.g = g;
        this.b = b;
    }
    /**
     * Return the alpha component of a color int. This is the same as saying
     * color >>> 24
     *
     */
    public void setHUVColor(int h,int u, int v){
        int color = Color.HSVToColor(new float[]{h,u,v});
        r = Color.red(color);
        g = Color.green(color);
        b = Color.blue(color);
    }

}

package net.redbear.redbear8nanos;

import com.google.gson.Gson;
import com.google.gson.annotations.SerializedName;
import static net.redbear.redbear8nanos.Common.*;

public class DataPackage {
    @SerializedName("ID")
    private int id;
    @SerializedName("OpCode")
    private int opCode;
    @SerializedName("NUM")
    private int num;
    @SerializedName("R")
    private int r;
    @SerializedName("G")
    private int g;
    @SerializedName("B")
    private int b;

    public DataPackage pack(RGBDevice device,int opCode){
        this.id = device.getId();
        this.opCode = opCode;
        this.num = 0;
        this.r = device.getR();
        this.g = device.getG();
        this.b = device.getB();
        return this;
    }

    public int getId() {
        return id;
    }

    public int getOpCode() {
        return opCode;
    }

    public int getNum() {
        return num;
    }

    public int getR() {
        return r;
    }

    public int getG() {
        return g;
    }

    public int getB() {
        return b;
    }

    public String packOFFString(Gson gson, int opCode, int id){
        this.id = id;
        this.opCode = opCode;
        r = 0;
        g = 0;
        b = 0;
        return gson.toJson(this);
    }

    public String getNanoCountJsonString(Gson gson){
        opCode = OPCODE_GET_DEV_COUNT;
        id = DEV_COUNT_FLG;
        num = 0;
        r = 0;
        g = 0;
        b = 0;
        return gson.toJson(this);
    }

}

package ccfit.nsu.daryaevd;

import com.google.gson.annotations.SerializedName;

public class Wind {
    @SerializedName("speed")
    public double speed;

    public double getSpeed() {
        return speed;
    }
}

package ccfit.nsu.daryaevd;

import com.google.gson.annotations.SerializedName;


public class Weather {
    @SerializedName("main")
    public String mainWeather;

    @SerializedName("description")
    public String description;

    public String getMainWeather() {
        return mainWeather;
    }
    public String getDescription() {
        return description;
    }
}

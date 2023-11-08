package ccfit.nsu.daryaevd.weather.components;

import com.google.gson.annotations.SerializedName;

public final class OpenWeatherMain {
    @SerializedName("temp")
    public double realTemp;

    @SerializedName("feels_like")
    public double feelsLike;

    @SerializedName("pressure")
    public double pressure;

    @SerializedName("humidity")
    public double humidity;

    public double getRealTemp() {
        return realTemp;
    }

    public double getFeelsLike() {
        return feelsLike;
    }

    public double getPressure() {
        return pressure;
    }

    public double getHumidity() {
        return humidity;
    }

}

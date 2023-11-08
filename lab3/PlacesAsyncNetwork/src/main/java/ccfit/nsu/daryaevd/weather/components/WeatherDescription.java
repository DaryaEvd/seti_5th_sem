package ccfit.nsu.daryaevd.weather.components;

import com.google.gson.annotations.SerializedName;

import java.util.List;

public class WeatherDescription {
    @SerializedName("weather")
    public List<Weather> weatherList;

    @SerializedName("main")
    public OpenWeatherMain openWeatherMain;

    @SerializedName("visibility")
    public String visibility;

    @SerializedName("wind")
    public Wind wind;

    public String getMainWeatherInWeather() {
        return weatherList.get(0).getMainWeather();
    }

    public String getWeatherDescription() {
        return weatherList.get(0).getDescription();
    }

    public double getRealTemp() {
        return openWeatherMain.getRealTemp() - 273.15;
    }

    public double getTempFeelsLike() {
        return openWeatherMain.getFeelsLike() - 273.15;
    }

    public double getPressure() {
        return openWeatherMain.getPressure() / 1000; // from kBar to millimeters of mercury
    }

    public double getHumidity() {
        return openWeatherMain.getHumidity();
    }

    public String getVisibility() {
        return visibility;
    }

    public double getWindSpeed() {
        return wind.getSpeed();
    }

}

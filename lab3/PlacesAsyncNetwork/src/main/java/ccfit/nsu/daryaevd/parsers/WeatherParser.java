package ccfit.nsu.daryaevd.parsers;

import ccfit.nsu.daryaevd.weather.components.WeatherDescription;
import com.google.gson.Gson;

public class WeatherParser {
    public static WeatherDescription parseWeather(String jsonInput) {
        Gson gson = new Gson();
        return gson.fromJson(jsonInput, WeatherDescription.class);
    }
}

package ccfit.nsu.daryaevd.parsers;

import ccfit.nsu.daryaevd.places.componetns.DescriptionPlace;
import com.google.gson.Gson;

public class DescriptionPlaceParser {
    public static DescriptionPlace parseDescriptionPlace(String jsonInput) {
        Gson gson = new Gson();
        return gson.fromJson(jsonInput, DescriptionPlace.class);
    }
}

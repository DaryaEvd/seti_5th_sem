package ccfit.nsu.daryaevd;

import com.google.gson.Gson;

import java.util.ArrayList;

public class GeocodeLocationParser {
    public static ArrayList<InfoLocation> parseLocation(String jsonInput) {
        Gson gson = new Gson();
        HitItems hitItems = gson.fromJson(jsonInput, HitItems.class);
        return hitItems.hits();
    }

}

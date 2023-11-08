package ccfit.nsu.daryaevd.parsers;

import ccfit.nsu.daryaevd.location.components.ListOfPlaces;
import ccfit.nsu.daryaevd.places.componetns.InterestingPlace;
import com.google.gson.Gson;

import java.util.ArrayList;

public class PlaceParser {
    public static ArrayList<ListOfPlaces> parsePlace(String jsonInput) {
       Gson gson = new Gson();
       InterestingPlace places = gson.fromJson(jsonInput, InterestingPlace.class);
       return places.features();
    }
}

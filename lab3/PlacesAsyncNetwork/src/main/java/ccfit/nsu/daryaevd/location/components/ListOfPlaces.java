package ccfit.nsu.daryaevd.location.components;

import com.google.gson.annotations.SerializedName;

public class ListOfPlaces {
    @SerializedName("id")
    String id;

    public ListOfPlaces(String id) {
        this.id = id;
    }

    public String id() {
        return id;
    }

    @Override
    public String toString() {
        return "ListOfPlaces[" +
                "id=" + id + ']';
    }

}

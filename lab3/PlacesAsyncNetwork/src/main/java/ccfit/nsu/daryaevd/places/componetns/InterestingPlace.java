package ccfit.nsu.daryaevd.places.componetns;

import ccfit.nsu.daryaevd.location.components.ListOfPlaces;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.Objects;

public class InterestingPlace {
    @SerializedName("features")
    ArrayList<ListOfPlaces> features;

    public InterestingPlace(ArrayList<ListOfPlaces> features) {
        this.features = features;
    }

    public ArrayList<ListOfPlaces> features() {
        return features;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == this) return true;
        if (obj == null || obj.getClass() != this.getClass()) return false;
        var that = (InterestingPlace) obj;
        return Objects.equals(this.features, that.features);
    }

    @Override
    public int hashCode() {
        return Objects.hash(features);
    }

    @Override
    public String toString() {
        return "InterestingPlace[" +
                "features=" + features + ']';
    }

}

package ccfit.nsu.daryaevd.location.components;

import com.google.gson.annotations.SerializedName;

public class InfoLocation {
    @SerializedName("point")
    public Point point;

    @SerializedName("name")
    public String name;

    @SerializedName("country")
    public String country;

    @SerializedName("city")
    public String city;

    @SerializedName("osm_value")
    public String osmValue;

    @SerializedName("osm_key")
    public String osmKey;

    public InfoLocation(Point point, String name, String country, String city, String osmValue, String osmKey) {
        this.point = point;
        this.name = name;
        this.country = country;
        this.city = city;
        this.osmValue = osmValue;
        this.osmKey = osmKey;
    }

    public String getName() {
        if (name == null) {
            return "";
        }
        return "Name: " + name;
    }

    public String getCountry() {
        if (country == null) {
            return "";
        }
        return "Country: " + country;
    }

    public String getCity() {
        if (city == null) {
            return "";
        }
        return "City: " + city;
    }

    public String getOsmKey() {
        if (osmKey == null) {
            return "";
        }
        return "More detailed: " + osmKey;
    }

    public String getOsmValue() {
        if (osmKey == null) {
            return "";
        }
        return ", " + osmValue;
    }

}

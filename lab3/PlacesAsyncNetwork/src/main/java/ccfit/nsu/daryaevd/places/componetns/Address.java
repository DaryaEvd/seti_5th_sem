package ccfit.nsu.daryaevd.places.componetns;

import com.google.gson.annotations.SerializedName;

public class Address {
    @SerializedName("road")
    public String road;
    @SerializedName("city_district")
    public String cityDistrict;
    @SerializedName("house_number")
    public String houseNumber;

    public Address(String road, String cityDistrict, String houseNumber) {
        this.road = road;
        this.cityDistrict = cityDistrict;
        this.houseNumber = houseNumber;
    }

    public String getRoad() {
        return road;
    }

    public String getCityDistrict() {
        return cityDistrict;
    }

    public String getHouseNumber() {
        return houseNumber;
    }

    @Override
    public String toString() {
        return "Address{" +
                "road='" + road + '\'' +
                ", cityDistrict='" + cityDistrict + '\'' +
                ", houseNumber='" + houseNumber + '\'' +
                '}';
    }
}

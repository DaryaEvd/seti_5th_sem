package ccfit.nsu.daryaevd;


import java.util.List;

public class AdditionalInfoLocation {
    WeatherDescription weatherDescription;
    List<PlaceDescription> placeDescriptionList;

    AdditionalInfoLocation(WeatherDescription weatherDescription, List<PlaceDescription> placeDescription) {
       this.weatherDescription = weatherDescription;
       this.placeDescriptionList = placeDescription;
    }
}

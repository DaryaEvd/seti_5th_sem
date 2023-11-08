package ccfit.nsu.daryaevd.places.componetns;


import ccfit.nsu.daryaevd.places.componetns.DescriptionPlace;
import ccfit.nsu.daryaevd.weather.components.WeatherDescription;

import java.util.List;

public class AdditionalInfoLocation {
    public WeatherDescription weatherDescription;
    public List<DescriptionPlace> placeDescriptionList;

    public AdditionalInfoLocation(WeatherDescription weatherDescription, List<DescriptionPlace> placeDescription) {
        this.weatherDescription = weatherDescription;
        this.placeDescriptionList = placeDescription;
    }
}

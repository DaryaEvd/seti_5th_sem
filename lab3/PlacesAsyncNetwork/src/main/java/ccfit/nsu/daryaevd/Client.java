package ccfit.nsu.daryaevd;

import ccfit.nsu.daryaevd.parsers.GeocodeLocationParser;
import ccfit.nsu.daryaevd.location.components.InfoLocation;
import ccfit.nsu.daryaevd.location.components.ListOfPlaces;
import ccfit.nsu.daryaevd.location.components.Point;
import ccfit.nsu.daryaevd.parsers.DescriptionPlaceParser;
import ccfit.nsu.daryaevd.parsers.PlaceParser;
import ccfit.nsu.daryaevd.places.componetns.DescriptionPlace;
import ccfit.nsu.daryaevd.weather.components.WeatherDescription;
import ccfit.nsu.daryaevd.parsers.WeatherParser;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;

public class Client {

    private static final String GEOCODE_START_REQUEST = "https://graphhopper.com/api/1/geocode?q=";
    private static final String GEOCODE_ADDITION = "&locale=en";
    private static final String GEOCODE_LIMIT_MAX = "&limit=";
    private static final String GEOCODE_API = "606d0fc3-813d-43fc-8c19-f9e6666f0677";
    private static final String WEATHER_START_REQUEST = "https://api.openweathermap.org/data/2.5/weather?";
    private static final String WEATHER_API_KEY = "1eb43fdc22ea2444d0a7a5e8ad2035e8";
    private static final String INTERESTING_PLACES_START_REQUEST = "https://api.opentripmap.com/0.1/en/places/radius?radius=";
    private static final String INTERESTING_PLACES_API_KEY = "5ae2e3f221c38a28845f05b68dec3ca088be1fb35e6091d4ab1a337a";
    private static final int RADUIS = 2500;

    CompletableFuture<ArrayList<InfoLocation>> getLocationsFromAPI(String locationUser) {
        int limit = 10;
        String URL_GEOCODE = GEOCODE_START_REQUEST + locationUser + GEOCODE_ADDITION + GEOCODE_LIMIT_MAX + limit + "&key=" + GEOCODE_API;
        System.out.println("geocode:" + URL_GEOCODE);

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_GEOCODE))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(GeocodeLocationParser::parseLocation);
    }

    public CompletableFuture<WeatherDescription> getWeatherFromAPI(Point point) {
        String URL_WEATHER = WEATHER_START_REQUEST + "lat=" + point.lat() + "&lon=" + point.lng() + "&appid=" + WEATHER_API_KEY;

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_WEATHER))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(WeatherParser::parseWeather);
    }

    CompletableFuture<ArrayList<ListOfPlaces>> getInterestingPlacesFromAPI(Point point) {
        String URL_INTERESTING_PLACES = INTERESTING_PLACES_START_REQUEST + RADUIS
                + "&lon=" + point.lng()
                + "&lat=" + point.lat()
                + "&format=geojson&apikey=" + INTERESTING_PLACES_API_KEY;

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_INTERESTING_PLACES))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(PlaceParser::parsePlace);
    }

    CompletableFuture<DescriptionPlace> getDetailsAboutInterestingPlacesFromAPI(String id) {
        String URL_DESCRIPTION_INTERESTING_PLACES =
                "http://api.opentripmap.com/0.1/ru/places/xid/"
                        + id
                        + "?apikey=" + INTERESTING_PLACES_API_KEY;

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_DESCRIPTION_INTERESTING_PLACES))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(DescriptionPlaceParser::parseDescriptionPlace);

    }
}

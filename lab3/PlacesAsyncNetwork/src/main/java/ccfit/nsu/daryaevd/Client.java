package ccfit.nsu.daryaevd;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;

public class Client {

    private static final String GEOCODE_START_REQUEST = "https://graphhopper.com/api/1/geocode?q=";
    private static final String GEOCODE_ADDITION = "&locale=en&key=";
    private static final String GEOCODE_API = "606d0fc3-813d-43fc-8c19-f9e6666f0677";
    private static final String WEATHER_START_REQUEST = "https://api.openweathermap.org/data/2.5/weather?";
    private static final String WEATHER_API_KEY = "1eb43fdc22ea2444d0a7a5e8ad2035e8";


    CompletableFuture<ArrayList<InfoLocation>> getLocationsFromAPI(String locationUser) {
        String URL_GEOCODE = GEOCODE_START_REQUEST + locationUser + GEOCODE_ADDITION + GEOCODE_API;

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_GEOCODE))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApply(resp -> {
                    if (resp.statusCode() != 200) {
                        System.err.println("response code is not 200");
                    }
                    return resp;
                })
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(GeocodeLocationParser::parseLocation);
    }


    public CompletableFuture<WeatherDescription> getWeather(Point point) {
        String URL_WEATHER = WEATHER_START_REQUEST + "lat=" + point.lat() + "&lon=" + point.lng() + "&appid=" + WEATHER_API_KEY;
        System.out.println("Url weather: " + URL_WEATHER);

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_WEATHER))
                .GET()
                .build();

        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApply(
                        resp -> {
                            if (resp.statusCode() != 200) {
                                System.err.println("response code is not 200");
                            }
                            return resp;
                        }
                )
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(WeatherParser::parseWeather);
    }
}

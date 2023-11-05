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


    CompletableFuture<ArrayList<InfoLocation>> getLocationsFromAPI(String locationUser) {
        String URL_GEOCODE = GEOCODE_START_REQUEST + locationUser + GEOCODE_ADDITION + GEOCODE_API;

        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(URL_GEOCODE))
                .GET()
                .build();
        return client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApplyAsync(HttpResponse::body)
                .thenApplyAsync(GeocodeLocationParser::parseLocation);
    }


}

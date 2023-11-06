package ccfit.nsu.daryaevd;

import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class Main {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        Scanner scannerInput = new Scanner(System.in);
        System.out.println("Enter a location: ");
        String locationUser = scannerInput.nextLine();

        Client client = new Client();

        ArrayList<InfoLocation> locationsList = client.getLocationsFromAPI(locationUser).get();
        if (locationsList.isEmpty()) {
            System.out.println("No location found");
            return;
        }
        printLocationsList(locationsList);

        int numberOfLocation = chooseNumberOfLocation(scannerInput, locationsList.size());
        System.out.println("number of location: " + numberOfLocation);
//        numberOfLocation--;
        printChosenPlace(locationsList, numberOfLocation - 1);

        System.out.println("index: " + numberOfLocation);
        Point point = locationsList.get(numberOfLocation - 1).point;
//        CompletableFuture<AdditionalInfoLocation> additionalInfoLocation = getAdditionalInfoAboutPlace(point, client);

        WeatherDescription weatherDescription = client.getWeather(point).get();
        printWeatherDescription(weatherDescription);
    }

    private static void printWeatherDescription(WeatherDescription description) {
        System.out.println("main weather: " + description.getMainWeatherInWeather());
        System.out.println("more info about weather: " + description.getWeatherDescription());

        String realTemp = String.format("%.2f C", description.getRealTemp());
        System.out.println("real temp: " + realTemp);
        String tempFeelsLike = String.format("%.2f C", description.getTempFeelsLike());
        System.out.println("feels like: " + tempFeelsLike);

        System.out.println("pressure: " + description.getPressure());
        System.out.println("humidity: " + description.getHumidity());
        System.out.println("visibility: " + description.getVisibility());
        System.out.println("wind speed: " + description.getWindSpeed());
    }

    private static void printChosenPlace(ArrayList<InfoLocation> locationsList, int index) {
        System.out.println(locationsList.get(index).name);
        System.out.println(locationsList.get(index).country);
        System.out.println(locationsList.get(index).point.lat());
        System.out.println(locationsList.get(index).point.lng());

    }

    private static int chooseNumberOfLocation(Scanner scannerInput, int size) {
        System.out.println("Choose number of place you're interested in");
        while (true) {
            int numberToChoose = scannerInput.nextInt();
            if (numberToChoose > size || numberToChoose <= 0) {
                System.out.println("Incorrect number, choose another one");
            } else {
                return numberToChoose;
            }
        }
//        return ?? TODO: play with bounds
    }

    private static void printLocationsList(ArrayList<InfoLocation> locationsList) {
        int i = 0;
        for (InfoLocation location : locationsList) {
            System.out.println((++i) + ". " +
                    location.name + " [" +
                    location.point.lat() + ", " +
                    location.point.lng() + "], " +
                    location.country
            );
        }
    }
}
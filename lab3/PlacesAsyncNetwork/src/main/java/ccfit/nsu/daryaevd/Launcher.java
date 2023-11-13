package ccfit.nsu.daryaevd;

import ccfit.nsu.daryaevd.location.components.*;
import ccfit.nsu.daryaevd.places.componetns.*;
import ccfit.nsu.daryaevd.weather.components.*;

import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

import static java.util.stream.Collectors.collectingAndThen;
import static java.util.stream.Collectors.toList;

public class Launcher {

    public void runFindingPLaces() throws ExecutionException, InterruptedException {
        Scanner scannerInput = new Scanner(System.in);
        String locationUser = getValidInputToGraphHopper(scannerInput);

        Client client = new Client();

        ArrayList<InfoLocation> locationsList = client.getLocationsFromAPI(locationUser).get();
        if (locationsList.isEmpty()) {
            System.out.println("Unfortunately, No location found");
            return;
        }
        printLocationsList(locationsList);

        int numberOfLocation = chooseNumberOfLocation(scannerInput, locationsList.size());
        System.out.println("number of chosen location: " + numberOfLocation);
        printChosenPlace(locationsList, numberOfLocation - 1);

        Point point = locationsList.get(numberOfLocation - 1).point;

        CompletableFuture<AdditionalInfoLocation> additionalInfoLocation = getAdditionalInfoAboutPlace(point, client);

        printAdditionalInfoAboutLocation(additionalInfoLocation.join());

    }

    static String getValidInputToGraphHopper(Scanner userInput) {
        String location;
        while (true) {
            System.out.println("Enter a location: ");
            location = userInput.nextLine().trim().replaceAll("\\s+", " "); // no spaces at start and end,
            // and replace multiple spaces with a single space
            if (location.isEmpty()) {
                System.err.println("Location cannot be empty. Please enter a valid location.");
            } else {
                break; // input is valid
            }
        }
        return location.replace(" ", "-"); // replace spaces with dashes
    }

    private static int chooseNumberOfLocation(Scanner scannerInput, int size) {
        System.out.println("Choose number of place you're interested in");
        int numberToChoose;
        while (true) {
            if (scannerInput.hasNextInt()) {
                numberToChoose = scannerInput.nextInt();
                if (numberToChoose > size || numberToChoose <= 0) {
                    System.err.println("Incorrect number, please choose another one");
                } else {
                    break; // if input is valid
                }
            } else {
                System.err.println("Invalid input. Please enter a number.");
                scannerInput.next(); // invalid input
            }
        }
        return numberToChoose;
    }

    private static void printLocationsList(ArrayList<InfoLocation> locationsList) {
        int i = 0;
        for (InfoLocation location : locationsList) {
            System.out.println((++i) + ". " +
                    location.getName() + " [" +
                    location.point.lat() + ", " +
                    location.point.lng() + "], "
                    + location.getCountry() + " "
                    + location.getCity() + " "
                    + location.getOsmKey()
                    + location.getOsmValue()
            );
        }
    }

    private static void printChosenPlace(ArrayList<InfoLocation> locationsList, int index) {
        System.out.println(locationsList.get(index).getName());
        System.out.println(locationsList.get(index).getCountry());
        System.out.println(locationsList.get(index).getCity());
        System.out.println("Points: " + locationsList.get(index).point.lat() + ", "
                + locationsList.get(index).point.lng());
        System.out.print(locationsList.get(index).getOsmKey());
        System.out.println(locationsList.get(index).getOsmValue());
    }

    private static void printAdditionalInfoAboutLocation(AdditionalInfoLocation additionalInfoLocation) {
        System.out.println("=======================");
        printWeatherDescription(additionalInfoLocation.weatherDescription);
        System.out.println("=======================");
        System.out.println("Interesting places: " + "\n");

        for (DescriptionPlace descriptionPlace : additionalInfoLocation.placeDescriptionList) {
            if (!Objects.equals(descriptionPlace.getName(), "")
                    && descriptionPlace.getName() != null) {
                System.out.println(descriptionPlace);
            }
        }
    }

    private static CompletableFuture<AdditionalInfoLocation> getAdditionalInfoAboutPlace(Point point, Client client) {
        CompletableFuture<WeatherDescription> weatherDescription = client.getWeatherFromAPI(point);

        CompletableFuture<List<DescriptionPlace>> descriptionPlace =
                client.getInterestingPlacesFromAPI(point).thenCompose(places -> myAllOf(places.stream()
                        .map(ListOfPlaces::id)
                        .map(client::getDetailsAboutInterestingPlacesFromAPI).toList()));

        CompletableFuture<Void> all = CompletableFuture.allOf(descriptionPlace, weatherDescription);
        // work with weather and description place, wait their finishing
        return all.thenApply(x -> new AdditionalInfoLocation(weatherDescription.join(), descriptionPlace.join()));
        // create new object with weather and description place

    }

    public static <T> CompletableFuture<List<T>> myAllOf(List<CompletableFuture<T>> places) {
        return places.stream()
                .collect(collectingAndThen(toList(), l -> CompletableFuture.allOf(l.toArray(new CompletableFuture[0]))
                        // wait all finishing all Completable Future objects
                        .thenApply(__ -> l.stream() // when all objects have finished
                                .map(CompletableFuture::join)
                                .collect(Collectors.toList()))));
    }

    private static void printWeatherDescription(WeatherDescription description) {
        System.out.println("main weather: " + description.getMainWeatherInWeather());
        System.out.println("more info about weather: " + description.getWeatherDescription());

        String realTemp = String.format("%.2f C", description.getRealTemp());
        System.out.println("real temp: " + realTemp);
        String tempFeelsLike = String.format("%.2f C", description.getTempFeelsLike());
        System.out.println("feels like: " + tempFeelsLike);

        System.out.println("pressure: " + String.format("%.2f mmHg", description.getPressure())); //" millimeters of mercury");
        System.out.println("humidity: " + description.getHumidity() + " %");
        System.out.println("visibility: " + description.getVisibility() + " meters");
        System.out.println("wind speed: " + description.getWindSpeed() + " meters per second");
    }

}

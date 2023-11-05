package ccfit.nsu.daryaevd;

import java.util.ArrayList;
import java.util.Scanner;
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
    }

    private static void printLocationsList(ArrayList<InfoLocation> locationsList) {
        int i = 0;
        for (InfoLocation location : locationsList) {
            System.out.println((i++) + ". " +
                    location.name + " [" +
                    location.point.lat() + ", " +
                    location.point.lng() + "], " +
                    location.country
            );
        }
    }
}
package ccfit.nsu.daryaevd.places.componetns;

public record InfoOpenTrip(String descr) {
    @Override
    public String toString() {
        return "\n From InfoOpenTrip: " + "\n"
                + "Description: " + descr + "\n";
    }
}

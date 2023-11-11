package ccfit.nsu.daryaevd.places.componetns;

public record Wiki(String title, String text) {
    @Override
    public String toString() {
        return "From Wiki: " + "\n"
                + "name: '" + title + "'\n"
                + "description: " + "\n"
                + text + "\n";
    }
}

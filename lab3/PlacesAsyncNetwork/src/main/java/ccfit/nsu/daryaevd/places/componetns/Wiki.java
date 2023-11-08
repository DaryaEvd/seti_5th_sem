package ccfit.nsu.daryaevd.places.componetns;

public record Wiki(String title, String text) {
    @Override
    public String toString() {
        return "From Wiki, "
                + "name: '" + title + "'\n" +
                "Description: '" + text + "'\n";
    }
}

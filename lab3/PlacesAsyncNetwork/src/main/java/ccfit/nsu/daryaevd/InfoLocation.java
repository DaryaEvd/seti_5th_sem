package ccfit.nsu.daryaevd;

import java.util.Objects;

public class InfoLocation {
    public final Point point;
    public final String name;
    public final String country;

    @Override
    public String toString() {
        return "InfoLocation{" +
                "point=" + point +
                ", name='" + name + '\'' +
                ", country='" + country + '\'' +
                '}';
    }

    public InfoLocation(Point point, String name, String country) {
        this.point = point;
        this.name = name;
        this.country = country;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        InfoLocation that = (InfoLocation) o;
        return Objects.equals(point, that.point)
                && Objects.equals(name, that.name)
                && Objects.equals(country, that.country);
    }

    @Override
    public int hashCode() {
        return Objects.hash(point, name, country);
    }
}

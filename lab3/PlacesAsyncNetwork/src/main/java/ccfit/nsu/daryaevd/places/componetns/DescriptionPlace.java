package ccfit.nsu.daryaevd.places.componetns;

import com.google.gson.annotations.SerializedName;

import org.apache.commons.lang.WordUtils;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

public class DescriptionPlace {
    @SerializedName("name")
    String name;
    @SerializedName("wikipedia_extracts")
    Wiki wikiExtracts;
    @SerializedName("info")
    InfoOpenTrip infoOpenTrip;
    @SerializedName("address")
    Address address;

    public DescriptionPlace(String name, Wiki wikiExtracts, InfoOpenTrip infoOpenTrip, Address address) {
        this.name = name;
        this.wikiExtracts = wikiExtracts;
        this.infoOpenTrip = infoOpenTrip;
        this.address = address;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("Name: ").append(name).append("\n");
        if (address.cityDistrict != null) {
            stringBuilder.append("Address is: ").append(address.cityDistrict);
        }
        if (address.road != null) {
            stringBuilder.append(", ").append(address.road);
        }
        if (address.houseNumber != null) {
            stringBuilder.append(", ").append(address.houseNumber);
        }

        stringBuilder.append("\n");

        if (wikiExtracts != null) {
            return WordUtils.wrap(stringBuilder.append(wikiExtracts).append("\n").toString(), 120);
        } else if (infoOpenTrip != null) {
            return "\n" + stringBuilder.append(WordUtils.wrap(cleanAndPrint(infoOpenTrip.toString()), 120)) + "\n";
        }
        return stringBuilder.append("Unfortunately, no description found\n\n").toString();

    }

    public static String cleanAndPrint(String htmlString) {
        Document doc = Jsoup.parse(htmlString);
        return doc.text();
    }

    public String getName() {
        return name;
    }

}

module ccfit.nsu.ru.daryaevd {
    requires javafx.controls;
    requires javafx.fxml;
    requires protobuf.java;

    opens ccfit.nsu.ru.daryaevd.lab4 to javafx.fxml;
    exports ccfit.nsu.ru.daryaevd.lab4;
}

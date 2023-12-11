package ccfit.nsu.ru.daryaevd.lab4;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.List;

import static java.lang.System.exit;

public class StartApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        List<String> args = getParameters().getRaw();
        if(args.size() != 2) {
            System.out.println("Usage: <port> <user's name>");
            exit(0);
        }

//        int port = Integer.parseInt(args.get(0));
//        String name = args.get(1);
//        NodeGame node = new NodeGame(port, name);

        FXMLLoader fxmlLoader = new FXMLLoader(StartApplication.class.getResource("hello-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load());


        stage.setTitle("SNAKE GAME!");
        stage.setResizable(false);
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}

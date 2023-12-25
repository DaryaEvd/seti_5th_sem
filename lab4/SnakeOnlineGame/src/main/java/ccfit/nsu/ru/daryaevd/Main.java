package ccfit.nsu.ru.daryaevd;

import ccfit.nsu.ru.daryaevd.model.Model;
import ccfit.nsu.ru.daryaevd.model.threads.GamesListener;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Main {
    public static void main(String[] args) {
        Logger logger = LoggerFactory.getLogger(Main.class.getSimpleName());
        logger.info("Game is starting!" );

        new Model();
    }
}

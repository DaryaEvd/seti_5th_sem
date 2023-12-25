package ccfit.nsu.ru.daryaevd.model.threads;

import ccfit.nsu.ru.daryaevd.model.Model;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class GameDisplayThread extends Thread {
    private static final Logger logger = LoggerFactory.getLogger(GameDisplayThread.class.getSimpleName());

    private boolean isRunning = true;
    private final Model model;

    public GameDisplayThread(Model model) {
        this.model = model;
    }

    @Override
    public void run() {
        while (isRunning) {
            try {
                sleep(1000);
            } catch (InterruptedException e) {
                logger.info("Thread interrupted");
            }
            model.showActiveGames();
        }
    }

    public void setStopped() {
        isRunning = false;
    }
}

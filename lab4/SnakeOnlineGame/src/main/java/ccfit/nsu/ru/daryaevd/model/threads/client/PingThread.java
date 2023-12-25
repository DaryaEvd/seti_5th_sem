package ccfit.nsu.ru.daryaevd.model.threads.client;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class PingThread extends Thread {
    private static final Logger logger = LoggerFactory.getLogger(PingThread.class.getSimpleName());

    private boolean isRunning = true;
    private final ClientThread parentThread;
    private final int delay;

    public PingThread(ClientThread parent, int delay) {
        this.parentThread = parent;
        this.delay = delay;
    }

    @Override
    public void run() {
        while (isRunning) {
            try {
                sleep(delay);
            } catch (InterruptedException e) {
                logger.info("{}: Ping thread interrupted", PingThread.class);
            }
            parentThread.sendPing();
        }
    }

    public void setStopped() {
        isRunning = false;
    }
}

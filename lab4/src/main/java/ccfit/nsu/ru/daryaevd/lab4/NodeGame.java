package ccfit.nsu.ru.daryaevd.lab4;

public class NodeGame extends Thread {
    public static final String MULTICAST_ADDR = "239.192.0.4";
    public static final int MULTICAST_PORT = 9192;

    String nodeName;
    int nodePort;

    NodeGame(int port, String userName) {
       nodePort = port;
       nodeName = userName;
    }
}

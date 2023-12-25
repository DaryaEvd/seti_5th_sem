package ccfit.nsu.ru.daryaevd.model.threads;

import ccfit.nsu.ru.daryaevd.model.FieldInfo;
import ccfit.nsu.ru.daryaevd.model.GameInfo;
import ccfit.nsu.ru.daryaevd.model.Model;
import ccfit.nsu.ru.daryaevd.model.threads.client.ClientThread;
import ccfit.nsu.ru.daryaevd.model.threads.master.MasterThread;
import protobuf.SnakesProto;

import java.net.DatagramSocket;
import java.net.SocketException;


public class MainThread {
    private final DatagramSocket socket;
    private final Model model;
    private final String playerName;
    private final FieldInfo fieldInfo;
    private final GameInfo gameInfo;
    private MasterThread master;
    private ClientThread client;
    private boolean isMaster;

    public MainThread(Model model, GameInfo gameInfo, String playerName) throws SocketException {
        socket = new DatagramSocket();
        this.model = model;
        this.playerName = playerName;

        this.gameInfo = gameInfo;
        fieldInfo = new FieldInfo(gameInfo.getConfig());
    }

    public void newGame() {
        master = new MasterThread(this, socket, model, gameInfo, fieldInfo, playerName);
        fieldInfo.setMasterThread(master);
        master.newGame();
        master.start();
        isMaster = true;
    }

    public void becomeMaster(int ID) {
        client.setStopped();
        master = new MasterThread(this, socket, model, gameInfo, fieldInfo, playerName);
        fieldInfo.setMasterThread(master);
        master.becomeMaster(ID);
        master.start();
        isMaster = true;
    }

    public void connectToGame(SnakesProto.NodeRole role) {
        client = new ClientThread(this, socket, model, gameInfo, fieldInfo, playerName);
        fieldInfo.setMasterThread(null);
        client.connectToGame(role);
        client.start();
        isMaster = false;
    }

    public void masterLost(int ID) {
        master.setStopped();
        client = new ClientThread(this, socket, model, gameInfo, fieldInfo, playerName);
        fieldInfo.setMasterThread(null);
        client.masterAsClient(ID);
        client.start();
        isMaster = false;
    }

    public void stop() {
        if (isMaster) {
            master.setStopped();
            master = null;
        } else {
            client.setStopped();
            client = null;
        }
    }

    public void makeMove(SnakesProto.Direction direction) {
        if (isMaster) {
            master.makeMove(direction);
        } else {
            client.makeMove(direction);
        }
    }

    public void disconnect() {
        if (isMaster) {
            master.disconnect();
        } else {
            client.disconnect();
        }
        socket.close();
    }

    public GameInfo getGameInfo() {
        return gameInfo;
    }

}

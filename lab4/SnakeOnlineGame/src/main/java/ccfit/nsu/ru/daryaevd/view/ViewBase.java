package ccfit.nsu.ru.daryaevd.view;

import ccfit.nsu.ru.daryaevd.view.gamepanel.GameFrame;
import ccfit.nsu.ru.daryaevd.view.initmenu.StartMenu;
import ccfit.nsu.ru.daryaevd.model.GameInfo;
import ccfit.nsu.ru.daryaevd.model.Model;
import ccfit.nsu.ru.daryaevd.model.Player;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.List;

public class ViewBase {
    private final Model model;
    private final StartMenu menu;
    private GameFrame gameFrame;

    public ViewBase(Model model) {
        this.model = model;

        menu = new StartMenu(this);
        menu.show();
    }

    public void showMessage(String message) {
        gameFrame.showMessage(message);
    }

    public void showActiveGames(List<GameInfo> games) {
        menu.showGames(games);
    }

    public void createGame(String gameName, String playerName) {
        GameInfo gameInfo;
        if ((gameInfo = model.createGame(gameName, playerName)) == null) {
            menu.showDialog("No free sockets");
        } else {
            menu.close();

            gameFrame = new GameFrame(this, gameInfo,
                    gameInfo.getConfig().getWidth(),
                    gameInfo.getConfig().getHeight());
        }
    }

    public void drawField(List<Player> players, List<Point> foods, String playerName) {
        gameFrame.drawField(players, foods, playerName);
    }

    public void makeMove(KeyEvent e) {
        model.makeMove(e);
    }

    public void disconnect() {
        model.disconnect();
        gameFrame.close();
        menu.show();
    }

    public void enterGame(GameInfo game, String gameName) {
        menu.dispose();
        gameFrame = new GameFrame(this, game,
                game.getConfig().getWidth(), game.getConfig().getHeight());
        model.connect(game, gameName);
    }

    public void exit() {
        model.stop();
        menu.close();
    }
}

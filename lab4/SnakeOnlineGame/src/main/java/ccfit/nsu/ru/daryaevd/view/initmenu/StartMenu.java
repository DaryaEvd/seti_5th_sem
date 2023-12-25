package ccfit.nsu.ru.daryaevd.view.initmenu;

import ccfit.nsu.ru.daryaevd.view.ViewBase;
import ccfit.nsu.ru.daryaevd.model.GameInfo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.List;

public class StartMenu implements ActionListener {
    private final ViewBase viewBase;
    private final JFrame mainWindow;
    private ActiveGamesPanel activeGames;

    public StartMenu(ViewBase viewBase) {
        this.viewBase = viewBase;

        mainWindow = new JFrame("MySnake");
        JPanel mainPanel = createStartPanelMenu();

        mainWindow.setContentPane(mainPanel);
        mainWindow.setResizable(false);
        mainWindow.setSize(new Dimension(600, 600));

        mainWindow.setLocationRelativeTo(null);
        mainWindow.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                exit();
                super.windowClosing(e);
            }
        });
    }

    private JPanel createStartPanelMenu() {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();
        constraints.insets = new Insets(5, 5, 5, 5);

        JLabel title = new JLabel("MySnake Online Game");
        title.setFont(new Font("Comic Sans MS", Font.BOLD, 36));
        constraints.gridx = 0;
        constraints.gridy = 0;
        constraints.gridwidth = 3;
        constraints.gridheight = 1;
        constraints.fill = GridBagConstraints.CENTER;
        title.setOpaque(true);
        title.setBackground(Color.decode("#730071"));
        title.setForeground(Color.WHITE);
        panel.add(title, constraints);

        JLabel gamesTitle = new JLabel("Available Games");
        gamesTitle.setFont(new Font("Comic Sans MS", Font.PLAIN, 22));
        constraints.gridy = 1;
        gamesTitle.setOpaque(true);
        gamesTitle.setBackground(Color.decode("#730071"));
        gamesTitle.setForeground(Color.WHITE);
        panel.add(gamesTitle, constraints);

        activeGames = new ActiveGamesPanel();
        constraints.gridy = 2;
        constraints.fill = GridBagConstraints.BOTH;
        activeGames.setBackground(Color.decode("#390040"));
        panel.add(activeGames, constraints);

        JButton newGameButton = new JButton("New game");
        newGameButton.setFont(new Font("Comic Sans MS", Font.PLAIN, 20));
        newGameButton.addActionListener(this);
        newGameButton.setActionCommand("createGame");
        constraints.gridy = 3;
        constraints.fill = GridBagConstraints.NONE;
        newGameButton.setBackground(Color.ORANGE);
        panel.add(newGameButton, constraints);

        JButton exitButton = new JButton("Exit");
        exitButton.setFont(new Font("Comic Sans MS", Font.PLAIN, 20));
        exitButton.addActionListener(this);
        exitButton.setActionCommand("exitGame");
        constraints.gridy = 4;
        exitButton.setBackground(Color.RED);
        panel.add(exitButton, constraints);

        constraints.gridy = 5;
        constraints.weighty = 1.0;
        panel.add(new JPanel(), constraints);

        panel.setBackground(Color.decode("#390040"));

        return panel;
    }

    public void showDialog(String message) {
        JOptionPane.showMessageDialog(mainWindow, message, "Info", JOptionPane.INFORMATION_MESSAGE);
    }

    public void showGames(List<GameInfo> games) {
        activeGames.removeGames();
        if (games.isEmpty()) {
            activeGames.draw();
        } else {
            for (GameInfo gameInfo : games) {
                activeGames.addGame(gameInfo, viewBase, mainWindow);
            }
            activeGames.draw();
        }

    }

    public void show() {
        mainWindow.setVisible(true);
    }

    public void dispose() {
        mainWindow.setVisible(false);
    }

    public void close() {
        mainWindow.dispose();
    }

    private void exit() {
        viewBase.exit();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        if ("createGame".equals(command)) {
            NewGameWindow gameWindow = new NewGameWindow(viewBase);
            gameWindow.show();
        } else if ("exitGame".equals(command)) {
            int result = JOptionPane.showConfirmDialog(mainWindow,
                    "Are you sure you want to exit?", "Confirm Exit", JOptionPane.YES_NO_OPTION);

            if (result == JOptionPane.YES_OPTION) {
                System.exit(0);
            }
        }
    }
}

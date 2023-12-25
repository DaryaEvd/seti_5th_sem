package ccfit.nsu.ru.daryaevd.view.initmenu;

import ccfit.nsu.ru.daryaevd.view.ViewBase;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class NewGameWindow implements ActionListener {
    private final ViewBase viewBase;
    private final JFrame mainWindow;
    private final JTextField gameName;
    private final JTextField playerName;

    public NewGameWindow(ViewBase viewBase) {
        this.viewBase = viewBase;
        mainWindow = new JFrame("Create a game");

        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();
        constraints.insets = new Insets(5, 5, 5, 5);

        JLabel gameNameTitle = new JLabel("Game name:");
        gameNameTitle.setFont(new Font("Comic Sans MS", Font.PLAIN, 16));
        constraints.gridx = 0;
        constraints.gridy = 0;
        panel.add(gameNameTitle, constraints);

        gameName = new JTextField(20);
        constraints.gridx = 1;
        panel.add(gameName, constraints);

        JLabel playerNameTitle = new JLabel("Player name:");
        playerNameTitle.setFont(new Font("Comic Sans MS", Font.PLAIN, 16));
        constraints.gridx = 0;
        constraints.gridy = 1;
        panel.add(playerNameTitle, constraints);

        playerName = new JTextField(20);
        constraints.gridx = 1;
        panel.add(playerName, constraints);

        JButton backButton = new JButton("Back");
        backButton.setBackground(Color.decode("#390040"));
        backButton.setForeground(Color.WHITE);
        backButton.setActionCommand("back");
        backButton.addActionListener(this);
        constraints.gridx = 0;
        constraints.gridy = 2;
        panel.add(backButton, constraints);

        JButton startButton = new JButton("Start");
        startButton.setBackground(Color.decode("#730071"));
        startButton.setForeground(Color.WHITE);
        startButton.setActionCommand("create");
        startButton.addActionListener(this);
        constraints.gridx = 1;
        panel.add(startButton, constraints);

        mainWindow.setContentPane(panel);
        mainWindow.setSize(new Dimension(400, 200));
        mainWindow.setLocationRelativeTo(null);

    }

    public void show() {
        mainWindow.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand().equals("create")) {
            if (gameName.getText().trim().isEmpty() || playerName.getText().trim().isEmpty()) {
                JOptionPane.showMessageDialog(mainWindow,
                        "Please enter valid values for game name and player name.",
                        "Error", JOptionPane.ERROR_MESSAGE);
            } else {
                String cleanedGameName = gameName.getText().trim().replaceAll("\\s+", " ");
                String cleanedPlayerName = playerName.getText().trim().replaceAll("\\s+", " ");

                viewBase.createGame(cleanedGameName, cleanedPlayerName);
                mainWindow.dispose();
            }

        } else if (e.getActionCommand().equals("back")) {
            mainWindow.dispose();
        }
    }
}

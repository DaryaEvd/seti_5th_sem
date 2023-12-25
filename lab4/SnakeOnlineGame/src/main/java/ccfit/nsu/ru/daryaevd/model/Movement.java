package ccfit.nsu.ru.daryaevd.model;

import protobuf.SnakesProto;

public class Movement {
    private final SnakesProto.Direction direction;
    private final int playerID;

    public Movement(SnakesProto.Direction direction, int playerID) {
        this.direction = direction;
        this.playerID = playerID;
    }

    public SnakesProto.Direction getDirection() {
        return direction;
    }

    public int getPlayerID() {
        return playerID;
    }

}

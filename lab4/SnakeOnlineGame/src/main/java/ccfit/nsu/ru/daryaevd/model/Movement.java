package ccfit.nsu.ru.daryaevd.model;

import protobuf.SnakesProto;

public record Movement(SnakesProto.Direction direction, int playerID) {

}

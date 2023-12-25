package ccfit.nsu.ru.daryaevd.model.threads;

import protobuf.SnakesProto;

public interface GameThread {
    void repeatMessage(SnakesProto.GameMessage gameMessage);

}

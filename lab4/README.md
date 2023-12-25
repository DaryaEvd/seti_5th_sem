## SNAKE ONLINE GAME  

### Task   
Implement a multiplayer game "Snake".  
While controlling your snake, you need to eat “food” and grow,   
while avoiding collisions with the “body” of your snake and with snakes controlled by other players.  
All student programs must implement the same protocol and be compatible with each other.  
The protocol description is in the [snakes.proto](https://github.com/DaryaEvd/seti_5th_sem/blob/main/lab4/SnakeOnlineGame/src/main/protobuf/snakes.proto) file.   
Next, the game config is mentioned; all config parameters are set before starting the game and do not change after that.  
Possible parameter values are specified in the protocol description file.  


### How to run  
**OFFLINE version**  
Open _IntlijIdea_ (or any other IDE), allow multiple instances of _Main.java_.  
Click on "Run" button to create as many instances as you want.  

**ONLINE**  
If you work on Windows, solve problem with brandmauer, that can block multicasting.   
I've used info from [this site](https://softbugs.ru/firewall-blocking-multicast/).  


### It might help  
I've watched [this guy](https://www.youtube.com/watch?v=ovdIp33GJdY&t=1718s) to understand protobuf.  
[This repo](https://github.com/elenakozlova/maven-protobuf.git) helped me with generating `java` file from `.proto`.  


### Rules of Game  
The playing field is a rectangle with sides specified in the game config.  
Let's call the horizontal rows of cells rows, and the vertical rows columns.  
In this case, the field is logically closed on opposite sides:   
from the leftmost cell of a row you can go left to the rightmost cell of the same row,   
similarly for the topmost cell of a column (mathematically, this is a discrete torus).  
A snake is a sequence of cells on the playing field, forming a continuous broken line   
with right bending angles (see figure).
Thus, the snake can cross the boundaries of the field an unlimited number of times.  
The cell at one end of the snake is the head, the direction of movement of which is   
controlled by the player, the cell at the opposite end is the tail.  
One or more snakes can be located on the field, each snake is controlled by one player.  
Also on the field at each moment of time there is food in an amount calculated by the   
formula (food_static + (number of ALIVE snakes)).  
Each food occupies 1 cell. There cannot be two food items on one cell.  
Food on the field is placed randomly on empty cells.  
If on the current turn there are not enough cells on the field to place the required  
amount of food, then as many as possible are placed.  

Time in the game is discrete, states change through a real time interval in milliseconds specified in the config.  
During the time between state changes (in one "turn"), players have the opportunity to change the direction of   
their snake's head using the SteerMsg command.  
If a command forces the snake to turn in the direction occupied by a cell adjacent to its head  
(an attempt to turn back), then such a command is ignored.  
The player can have time to replace the previous command by sending a new command, but if the command did not   
have time to reach the moment of state change, then it is applied on the next turn.  

At the moment of state change, the head of each snake moves 1 square in its current direction.  
If the cell where the head moved contained food, then the remaining cells of the snake do not move,   
and the player earns +1 point.
And if there was no food, then all subsequent cells of the snake move to the place of the previous ones,   
and the cell on which the tail was located is freed.
Next, the resulting state of the target cell where the snakes' heads have moved is checked.  
If the target cell is occupied by a snake (its own body, tail, or any other cell), then the “crashed”    
snake dies and the player is eliminated from the game.  
The cells occupied by a dead snake (after moving its head) turn into food with a probability of 0.5,   
and the remaining cells become empty.
A snake that has been “crashed” earns +1 point (if it hasn’t crashed into itself, of course).  
The snake dies regardless of whether the snake it “crashed” into dies on the same move.  
If several heads run into the same cage, then all these snakes die in the same way.  

In this way, you can, for example, closely pursue your own (or someone else’s) tail without dying.  
But if at the same time someone else’s snake eats food, then on this move we will “cram into” its tail.  
If there was food on the cage, and several heads ran into it at the same time, then the food is eaten,   
the tails of all these snakes remain in place, but then they all die.  

### Logic of program  
You can find it [here](https://gitlab.ccfit.nsu.ru/vadipp/04_snakes_task/-/blob/master/snakes_task.md).  

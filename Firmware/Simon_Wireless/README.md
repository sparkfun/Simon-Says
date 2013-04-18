Simon_Wireless.ino is a modification to the existing SparkFun Simon Says PTH kit code. It allows the user to connect an XBee on an XBee Explorer to the FTDI pins of the Simon Says board and play a wireless "Battle Mode" simon game with another player with the same set up. 

####Game Play:
The unit will power on and play an "attract" sequence. This will continue until one player presses a button. That player's unit will then go dark and wait for a first button in the sequence to be pressed. Once the user presses a button, the unit will send that sequence to the other player, play it back, and wait for the other player to then repeat it and add a button. Play continues in this fashion until one player incorrectly repeats the sequence.

####Required Parts:

* [Simon Says Through-Hole Soldering Kit](https://www.sparkfun.com/products/10547)
* [XBee](https://www.sparkfun.com/products/8666)
* [XBee Explorer Regulated](https://www.sparkfun.com/products/11373)
* [Right-Angle Male Pin Headers](https://www.sparkfun.com/products/553?)
* [Arduino Stackable Header - 6 Pin](https://www.sparkfun.com/products/9280?)


####Modification of Board:
In order for this code to work, a small modification of the Simon-Says board must take place. On the side of the board labeled "BOTTOM", one must solder a wire connecting the via on one side labeled "VCC" to a via on the other side. The via on the other side is the via that connects to VCC on the XBee Explorer. It is the via third in from the label "GRN". This modification provides power from the batteries to the XBee. 

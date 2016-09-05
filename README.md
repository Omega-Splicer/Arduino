This is the Arduino code for Omega-Splicer

usage :

  m1:int[;m2:int] -> controls motor 1 [and 2] 0 to 255
  

The module sends every SIGNAL_INTERVAL (defaults to (~ 8 seconds) :

  Approximate battery level in percentage from 3 to 4.2V eg : "b:16" for 3.2V. In int format
  
  Approximate signal strength eg : "s:-51" Value in int format
  
  Approximate temperature in Celsius by default eg : "t:25" Value in int format
  
  

Sketch uses 26,836 bytes (20%) of program storage space. Maximum is 131,072 bytes.

Global variables use 1,920 bytes of dynamic memory.


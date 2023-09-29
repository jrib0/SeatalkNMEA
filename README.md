# SeatalkNMEA
$STALK &lt;=> Seatalk with a Nano

Send $STALK NMEA0183 commands via the hardware UART on the NANO and it translates to 9 bit seatalk datagrams on a 9 bit software serial port and vice versa

NMEA-WIFIforSTALK is used to connect the DM-101 serial to wifi module so the NMEA comms are via WIFI

Many thanks to Thomas Knauf for the definitive resource of Seatalk information, Glinnes Hulden for NMEAparser and PappaD for Seatalkconverter (which I borrowed some of the code)

You will need an interface to convert the ST bus to RS232 levels.  I have used the one designed by Thomas Knauf http://www.thomasknauf.de/rap/seatalk3.htm

I admit that C is not my first language and so it may be error ridden and clunky.  I prefer Delphi/Pascal mainly because I am old fashioned!


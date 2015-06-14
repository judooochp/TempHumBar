# TempHumBar
Indoor/Outdoor Ambient Conditions Monitor

My best project to date (and most complete) is a less ambitious weather station which does not include a wind speed sensor or vane. I ran into issues with using two sensors of the same kind due to conflicting I2C addresses, and so had to implement a multiplexer and two lines of code to get what I wanted without much of a hassle.

It displays temperature in °F and °C, Relative Humidity (%RH), and Barometric pressure as Inches of Mercury ("Hg). Because I decided to create some kind of enclosure, I added the labels to the face itself instead of coding them in.

The hardware involved include two Adafruit BMP180 breakouts, two HTU21D-F breakouts, a Seeed Studio TFT Touch Shield (v2.0), a GD4051B multiplexer, and the Arduino Uno.

Future edits will include ranging adjustments to compensate for overflow when the temperature reaches 100 °F or -0.1 °C

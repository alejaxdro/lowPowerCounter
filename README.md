# lowPowerCounter

Low Powered Counter with LED for display ie Doodad
 
 Each time the button is pressed, the LED will blink the total number of counts saved. Holding the button down until the LED flashes quickly will add to the count. Restarting via the switch on the battery pack or pressing the reset button will reset the counter.
 
 The circuit:
 * Adafruit Trinket 5v - https://www.adafruit.com/product/1501
 * onBoard LED is tied to PIN 1
 * pushbutton attached from PIN 0 to PIN 2 (set HIGH)
 * 10K resistor attached from PIN 0 to GND - External Pull Down for input
 * 2 CR2032 in series provides ~6V to BATT and GND
 
I made this to help my wife count how many water bottles she had consumed in a day.

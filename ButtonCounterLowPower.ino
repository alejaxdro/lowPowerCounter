/* 
 Low Powered Counter with LED for display ie Doodad
 
 Each time the button is pressed, the LED will blink the total number of counts saved. Holding the button down until the LED flashes quickly will add to the count. Restarting via the switch on the battery pack or pressing the reset button will reset the counter.
 
 The circuit:
 * onBoard LED is tied to PIN 1
 * pushbutton attached from PIN 0 to PIN 2 (set HIGH)
 * 10K resistor attached from PIN 0 to GND - External Pull Down for input
 * 2 CR2032 in series provides ~6V to BATT and GND

 */
#include <avr/sleep.h>

// Utility macro
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarity
#define INTERRUPT_PIN PCINT0  // This is PB0 per the schematic
#define INT_PIN PB0           // Interrupt pin of choice: PB0 (same as PCINT0) - Pin 5

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 0;     // the number of the pushbutton pin
const int ledPin =  1;      // the number of the LED pin
const int highPin = 2;

// Variables will change:
boolean buttonActive = false;
boolean longPressActive = false;
bool ledState = false;         // the current state of the output pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long longPressTime = 1000;
int longpress_cnt = 0; // holds button long presses

ISR(PCINT_VECTOR)
{
  noInterrupts(); // cli();
}


void setup() {
  //pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(highPin, OUTPUT);
  digitalWrite(highPin, HIGH);
  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(PB0, INPUT);   // Set our interrupt pin as input with a pullup to keep it stable   
  
  adc_disable(); // ADC uses ~320uA
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  enterSleep();
}

void enterSleep(){
  //https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
  delay(50);
  sleep_enable();
  interrupts(); //sei();
  sleep_cpu();// on 2nd try comment this out
  
  /* The program will continue from here when it wakes up. */
  sleep_disable();
  
//  sei();
}

void blinkCnts(int l_delay, int l_cnt){
  for(int i = 0; i < l_cnt; i++){
      digitalWrite(ledPin, HIGH);
      delay(l_delay);
      digitalWrite(ledPin, LOW);
      delay(l_delay);
  }
}

void loop() {
  if(digitalRead(buttonPin) == HIGH){
    // Pressed
    if (buttonActive == false) {
      buttonActive = true;
      lastDebounceTime = millis();
      delay(1);
    }
    if (((millis() - lastDebounceTime) > longPressTime) && (longPressActive == false)) {
      // long press action
      longPressActive = true;
      longpress_cnt++;
      blinkCnts(50, 3);
    }
  }else{
    // Not Pressed
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
        enterSleep();
      } else {
        // short press action
        blinkCnts(150, longpress_cnt);
        enterSleep();
      }
      buttonActive = false;
    }
  }
  delay(1);
}

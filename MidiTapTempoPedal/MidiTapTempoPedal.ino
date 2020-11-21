/*
 * # MIDI Tap Tempo Pedal  
 * 
 * See the ReadMe (README.md)!  
 * 
 * Cheers, 
 * Owen Shartle  
 */

#include <MIDI.h>

// Serial: 0 RX + 1 TX.  
// Reads with RX.  
// Writes with TX.  
// We will be unable to communicate with the Arduino via the serial port (USB) 
// while the MIDI circuit is connected to RX.  
// If the software on the Arduino ever needs updated we must disconnect the RX and TX (pins 0 and 1) connections.  
MIDI_CREATE_DEFAULT_INSTANCE();

// TS (Tip Sleeve - 1/4" Guitar Cable) 5V signal pin.  
const byte tipSleevePin = 2;

// Tap Tempo Button pin.  
const byte tapTempoButtonPin = 3;

// Tempo signal pin.  
// Normally Closed (NC) when on (HIGH).  
// Normally Open (NO) when off (LOW).  
const byte normallyClosedTempoSignalPin = 4;

// Whether or not the tempo signal type is normally closed, or normally open.  
bool isNormallyClosedTempoSignal = false;

// Whether or not the tap tempo button is held.  
// You must release the button in order to trigger another tempo change signal.  
bool isTapTempoButtonHeld = false;

// MIDI values.  
const byte midiChannel = 1;
const byte tempoChangeMidiControlChangeNumber = 64;

void handleTempoSignalTypeSetting()
{
  isNormallyClosedTempoSignal = digitalRead(normallyClosedTempoSignalPin) == HIGH;
}

void triggerTempoChangeSignal()
{  
  digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? LOW : HIGH);
  delay(50);
  digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? HIGH : LOW);
}

void handleMidiControlChange(
  byte channel,
  byte number,
  byte velocity  
)
{
  if(
    channel == midiChannel
    && number == tempoChangeMidiControlChangeNumber
    && velocity > 0
  )
  {
    // Note that MIDI Thru will allow for all MIDI signals to be relayed.  
    // We only need to send the MIDI Out control change signal when the tempo button is pressed.  
    triggerTempoChangeSignal();
  }
}

void handleTapTempoButton()
{
  bool isTapTempoButtonPressed = digitalRead(tapTempoButtonPin) == HIGH;

  if(isTapTempoButtonPressed)
  {
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? LOW : HIGH);

    if(!isTapTempoButtonHeld)
    {
      isTapTempoButtonHeld = true;

      MIDI.sendControlChange(
        tempoChangeMidiControlChangeNumber,
        127,
        midiChannel
      );
    }
  }
  else
  {
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? HIGH : LOW);
    
    isTapTempoButtonHeld = false;
  }
}

void setup()
{
  // Start MIDI communications on all channels.  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(handleMidiControlChange);
  MIDI.turnThruOn();

  // Initialize the tap tempo button.  
  pinMode(tapTempoButtonPin, INPUT);

  // Initialize the tempo signal pin.  
  pinMode(normallyClosedTempoSignalPin, INPUT);

  // Initialize the 5V tempo signal pin.  
  pinMode(tipSleevePin, OUTPUT);
}

void loop()
{
  handleTempoSignalTypeSetting();
  MIDI.read();
  handleTapTempoButton();
}

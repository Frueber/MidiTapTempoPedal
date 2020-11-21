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

// Whether or not the tap tempo button is held.  
// You must release the button in order to trigger another tempo change signal.  
bool isTapTempoButtonHeld = false;

// MIDI values.  
const byte midiChannel = 1;
const byte tempoChangeMidiControlChangeNumber = 64;

// Whether or not the tempo change signal should be triggered (from either source).  
bool triggerTempoChangeSignalFromButtonPress = false;
bool triggerTempoChangeSignalFromMidi = false;

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
    triggerTempoChangeSignalFromMidi = true;
  }
}

void triggerTempoChangeSignal()
{
  // The Normally Open option is the default to avoid undesirable results such as harming a connected device.  
  bool isNormallyClosedTempoSignal = digitalRead(normallyClosedTempoSignalPin) == HIGH;
  
  digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? LOW : HIGH);
  digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? HIGH : LOW);
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

  // Set the appropriate button functionality.  
  triggerTempoChangeSignal();
}

void loop()
{
  MIDI.read();

  // Trigger MIDI Out, Thru, and 5V signal on guitar cable port.  
  // These events should be done as close together as possible.  
  
  if(
    !isTapTempoButtonHeld
    && digitalRead(tapTempoButtonPin) == HIGH
  )
  {
    triggerTempoChangeSignalFromButtonPress = true;
    isTapTempoButtonHeld = true;
  }
  else if(
    isTapTempoButtonHeld
    && digitalRead(tapTempoButtonPin) == LOW  
  )
  {
    isTapTempoButtonHeld = false;
  }

  if(triggerTempoChangeSignalFromMidi)
  {
    triggerTempoChangeSignal();

    // Note that MIDI Thru will allow for all MIDI signals to be relayed.  
    // We only need to send the MIDI Out control change signal when the tempo button is pressed.  
    
    triggerTempoChangeSignalFromMidi = false;
    triggerTempoChangeSignalFromButtonPress = false;
  }

  // We will prevent a MIDI message and tempo change button press from duplicating the signal.  
  // The MIDI trigger is given precedence in the cycle.  
  if(triggerTempoChangeSignalFromButtonPress)
  {
    triggerTempoChangeSignal();
    
    MIDI.sendControlChange(
      tempoChangeMidiControlChangeNumber,
      127,
      midiChannel
    );

    triggerTempoChangeSignalFromButtonPress = false;
  }
}

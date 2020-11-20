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
MIDI_CREATE_DEFAULT_INSTANCE();

// TS (Tip Sleeve - 1/4" Guitar Cable) 5V signal pin.  
const byte tipSleevePin = 2;

// Tap Tempo Button pin.  
const byte tapTempoButtonPin = 3;

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

void interruptTempoChangeSignal()
{
  digitalWrite(tipSleevePin, LOW);
  digitalWrite(tipSleevePin, HIGH);
}

void setup()
{
  // Start MIDI communications on all channels.  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(handleMidiControlChange);
  MIDI.turnThruOn();

  // Initialize the tap tempo button.  
  pinMode(tapTempoButtonPin, INPUT);
  
  // Initialize and set the 5V tempo signal pin.  
  pinMode(tipSleevePin, OUTPUT);
  digitalWrite(tipSleevePin, HIGH);
}

void loop()
{
  MIDI.read();

  // Trigger MIDI Out, Thru, and interrupt 5V signal on guitar cable port.  
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
    interruptTempoChangeSignal();

    // Note that MIDI Thru will allow for all MIDI signals to be relayed.  
    // We only need to send the MIDI Out control change signal when the tempo button is pressed.  
    
    triggerTempoChangeSignalFromMidi = false;
    triggerTempoChangeSignalFromButtonPress = false;
  }

  // We will prevent a MIDI message and tempo change button press from duplicating the signal.  
  // The MIDI trigger is given precedence in the cycle.  
  if(triggerTempoChangeSignalFromButtonPress)
  {
    interruptTempoChangeSignal();
    
    MIDI.sendControlChange(
      tempoChangeMidiControlChangeNumber,
      127,
      midiChannel
    );

    triggerTempoChangeSignalFromButtonPress = false;
  }
}

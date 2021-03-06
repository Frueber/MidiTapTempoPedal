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

// MIDI values.  
const byte midiChannel = 1;
const byte tempoChangeMidiControlChangeNumber = 64;

// Simulated tempo button press (tap) start time in milliseconds.  
unsigned long simulatedTempoTapStartTimeInMilliseconds = 0;

// Simulated tempo button press (tap) duration in milliseconds.  
// How long a simulated tempo tap will last.  
unsigned long simulatedTempoTapDurationInMilliseconds = 60;

void handleTempoSignalTypeSetting()
{
  isNormallyClosedTempoSignal = digitalRead(normallyClosedTempoSignalPin) == HIGH;
}

void handleSimulatedTempoTap()
{
  unsigned long timePassedSinceLastSimulatedTempoTapInMilliseconds = millis() - simulatedTempoTapStartTimeInMilliseconds;

  if(timePassedSinceLastSimulatedTempoTapInMilliseconds < simulatedTempoTapDurationInMilliseconds)
  {
    // Tempo button is pressed.  
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? LOW : HIGH);
  }
  else
  {
    // Tempo button is not pressed.  
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? HIGH : LOW);
  }
}

void handleMidiControlChange(
  byte channel,
  byte number,
  byte velocity  
)
{
  // Note that MIDI Thru will allow for all MIDI signals to be relayed.  
  // We only need to send the MIDI Out control change signal when the tempo button is pressed.  
  
  if(
    channel == midiChannel
    && number == tempoChangeMidiControlChangeNumber
    && velocity > 0
  )
  {
    unsigned long timePassedSinceLastSimulatedTempoTapInMilliseconds = millis() - simulatedTempoTapStartTimeInMilliseconds;

    if(timePassedSinceLastSimulatedTempoTapInMilliseconds >= simulatedTempoTapDurationInMilliseconds)
    {
      // Set bool or time to start tempo tap.  
      simulatedTempoTapStartTimeInMilliseconds = millis();
    }
  }
}

void handleTapTempoButton()
{
  bool isTapTempoButtonPressed = digitalRead(tapTempoButtonPin) == HIGH;

  if(isTapTempoButtonPressed)
  {
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? LOW : HIGH);

    MIDI.sendControlChange(
      tempoChangeMidiControlChangeNumber,
      127,
      midiChannel
    );
  }
  else
  {
    digitalWrite(tipSleevePin, isNormallyClosedTempoSignal ? HIGH : LOW);
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

  // Initialize the tempo signal type pin.  
  pinMode(normallyClosedTempoSignalPin, INPUT);

  // Initialize the 5V tempo signal pin.  
  pinMode(tipSleevePin, OUTPUT);
}

void loop()
{
  handleTempoSignalTypeSetting();
  MIDI.read();
  handleSimulatedTempoTap();
  handleTapTempoButton();
}

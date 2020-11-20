# MIDI Tap Tempo Pedal  

A guitar pedal that supports physical tempo taps and MIDI Out/Thru accordingly.  
The MIDI Tap Tempo Pedal can be any pedal-slingers on-beat bestie.  

The pedal accepts a standard, physically initiated, button press, or "tap", to set the tempo. Any two presses will set the tempo.  
Simple enough - what makes this special?  
The pedal can also accept a MIDI control change signal in place of a button press.  
This means you can take advantage of automated tap tempo changes even with pedals that don't directly support MIDI.  

When the pedal receives a MIDI Control Change #64 message, with a value greater than 0, it will relay the signal to the connected pedal(s).  
A standard tempo change signal, via the TS (1/4" guitar cable) jack, and a MIDI Control Change #64 message, through the MIDI Out/Thru port, are sent whenever the pedal ngaged".  

> The pedal is momentarily "engaged" when the button is pressed or when the expected MIDI Control Change message is received.  
> Note that the pedal supports MIDI Thru, and thus an exact copy of all received MIDI messages will be passed on via the MIDI Out/Thru port.  
> MIDI Control Change #64 matches the Tap Tempo message for Line 6 pedals M5/M9/M13 and potentially others.  

Author: Owen Shartle  

## Change Log  
- 11/19/2020: Created.  

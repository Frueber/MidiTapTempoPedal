# MIDI Tap Tempo Pedal  

Also known as **TOMATO** (Tap or MIDI Automation Tempo Operator).  

A guitar pedal that supports physical tempo taps and MIDI Out/Thru accordingly.  
The MIDI Tap Tempo Pedal can be any pedal-slingers on-beat bestie.  

---

## Introduction  

The pedal accepts a standard, physically initiated, button press, or "tap", to set the tempo. Any two presses will set the tempo.  
Simple enough - what makes this special?  
The pedal can also accept a MIDI control change signal in place of a button press.  
This means you can take advantage of automated tap tempo changes even with pedals that don't directly support MIDI.  

When the pedal receives a MIDI Control Change #64 message, with a value greater than 0, it will relay the signal to the connected pedal(s).  
A standard tempo change signal, via the TS (1/4" guitar/instrument cable) jack, and a MIDI Control Change #64 message, through the MIDI Out/Thru port, are sent whenever the pedal engaged".  

This version has targeted the Arduino Nano in order to be small and functional.  
It's possible that another Arduino board type could be used, such as the UNO or Mega.  

> - The pedal is momentarily "engaged" when the button is pressed or when the expected MIDI Control Change message is received.  
> - Note that the pedal supports MIDI Thru, and thus an exact copy of all received MIDI messages will be passed on via the MIDI Out/Thru port.  
> - MIDI Control Change #64 matches the Tap Tempo message for Line 6 pedals M5/M9/M13 and potentially others.  
> - We will be unable to communicate with the Arduino via the serial port (USB) while the MIDI circuit is connected to RX and TX.  
> If the software on the Arduino ever needs updated we must disconnect the RX and TX connections (pins 0 and 1). To do this, something like jumper caps can be used.  

---

## Hardware  

### Microprocessor  

- 1 Arduino Nano.  

### Ports  

- 1 TS Jack (Mono 1/4" Guitar Cable Jack).  
- 2 MIDI ports (5 pin).  
- 1 5.5mm x 2.1mm 9V DC electrical jack (chosen power source).  
  > Most tap tempo pedals won't need a power source as a 5V signal will come from the connected pedal. This version is opting for a separate 9V power source to ensure that the Arduino will always have enough power for operation with any connected device(s).  

### Peripherals  

- 1 guitar pedal foot switch.  
  > As the Arduino will handle the NC / NO (normally closed / normally open) circuit for button presses there isn't a need for a complex button that can support NC / NO or bypass circuitry. A simple two-pole momentary foot switch is perfect for the job.  

### Components  

- 3 220 Ohm resistors.  
- 1 4.7k Ohm resistor.  
- 1 10k Ohm resistor.  
- 1 Diode (1N4007).  
- 1 6N138 Optocoupler.  
- 2 Jumper caps.
  > Or a similar method of connecting/disconnecting pins.  
- 2 2-pin connectors.  
  > Used with the jumper caps to connect and disconnect the RX and TX pins.  
- 1 switch, such as a single DIP, slide, toggle, or on/off switch.  
  or  
  1 2-pin connector; an additional jumper cap would be needed.  
  > Used for switching between a NC or NO (normally closed/open) tempo signal. If connecting to a device via the instrument cable jack make sure to check the specification of that device before use; choose NC or NO accordingly.  

### Case  

- 1 Guitar Pedal Enclosure.  
  > **Suggestion**  
  > Enclosure 1590B.  
  > It's possible that the components and wiring could all fit inside the 1590A, 
  > but it will take some extra precision.  
  > Of course, any enclosure that fits the parts will work if you would like to use a different one.  

### Circuit Board  

If you're looking for a PCB (printed circuit board) option you can create your own or check out the included files here.  
A decent PCB designer tool is [EasyEDA](https://easyeda.com).  
After designing a board you can get the PCB created by uploading your files to any PCB manufacturer that works for you. One that is connected with EasyEDA is [JLCPCB](https://jlcpcb.com/).  
You can also wire and/or solder in any way you'd like. Feel free to use the PCB files here for reference.  

---

## Contributors  

Owen Shartle  

---

## Change Log  
- 11/19/2020: Created.  

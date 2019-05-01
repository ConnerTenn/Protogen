# Protogen

## System block diagram
---

### General Controller
- IO controller<br>
- Motor Controller<br>
- Power Distributor<br>

### Chest Controller
- Synth<br>
- Text Screen Control<br>

### Emotion Controller
- Facial Screen Control<br>
- Ear motor control<br>
- Body screen control<br>

<br>
### Synth

```
Inputs:
Note On/Off
 -Waveform
LFO
 -Waveform
Pitch Bend
ASDR
Filter
Loop
 -Record/Stop
 -Play/Stop

Velocity->ASDR, Volume
Pressure->Filter, Volume, LFO 

-------------------------------------
|                                   |
| Note   LFO,Bend  ASDR             |
|   |     |         |               |
|  Gate->Waveform->Envolope-+-->Out |
|  Gate->Waveform->Envolope-+       |
|              ...          +       |
|  Gate->Waveform->Envolope-+       |
-------------------------------------
   |
   v
--------------------
| Filter           |
| Low  High cutoff |
--------------------
   |
   V  
--------
| Loop |
--------

```
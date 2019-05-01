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
NoteOnOff
Waveform
LFOWaveform
PitchBend
ASDR
Filter
LoopRecord
LoopPlay

Velocity->ASDR, Volume
Pressure->Filter, Volume, LFO 

---------------------------------------
|                                     |
| LFO,Bend     Note    ASDR           |
|   |           |       |             |
|  Waveform--+-Gate->Envolope-+-->Out |
|            +-Gate->Envolope-+       |
|            +   ...          +       |
|            +-Gate->Envolope-+       |
---------------------------------------
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
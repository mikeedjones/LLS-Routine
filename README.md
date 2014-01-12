LLS-Routine
===========

Laser locking system routine for use with an arduino, as described in accompanying report. 

A programmable micro controller was used to both produce the initial sinusoidal waveform and to compute and return the error signal.

The Arduino implements an exclusive or (XOR) gate, taking as it's inputs the digitized derivatives of the absorption signal and the AC magnetic field, with a positive derivative corresponding to 1 and a negative derivative corresponding to 0. The error signal is determined from the XOR gate as follows: 

• When on resonance, the XOR gate spends as much time true as false. Over a full period of the magnetic field the average is 0.5 and no signal is sent to the laser. 

• When below resonance the XOR gate spends more time true than false, thus the average over a full period will be >0.5 and a positive signal is sent to drive the laser back to resonance. 

• When above resonance the XOR gate spends more time false than true, giving an average <0.5 over a full period causing a negative signal to be sent to the laser controller to drive the laser back to resonance.

# SparkTrackerUITest
Test UI changes

Install on ESP32 - Core 2 or Heltec or whatever - it only uses the serial monitor.   

This is  PoC to see if the ESP can make changes to the amp and get the UI to update. In a final version each change will just be a single effect change, which will force the UI update.   

The fudge is that is writes to preset 3 on the app and changes to preset 0 then preset 3 - which updates the UI for the full change.  The downside is that preset 3 is no longer available - although it still exists on the amp, selecting it will mean the app is out of sync.   

The sequence of events is:   
- turn off passthru so neither amp nor app get anything unexpected
- write a 'save current preset to hardware preset 3 on amp' to the app
- app then asks for the preset details
- this in intercepted by the ESP and it sends back the preset details
- ESP then sends app a 'change to preset 0' and then a 'change to preset 3' command
- UI refreshes

On Android there is a 'saved' pop-up - this is the best we can do.   
On IOS it is much nicer.


Start ESP and ensure it connects to spark

```
OK
Started
Found Spark
callback: Spark connected
connect_spark(): Spark connected
Starting classic bluetooth
Spark 40 Audio set up
Available for app to connect...
```

Then it retrieves the presets from the Spark amp

```
Message:  301  Send / receive new preset: 0
0 0 Mine : 2-Crunch
     bias.noisegate                  On  0.12 0.06 1.00 
     Compressor                      On  0.30 0.52 
     DistortionTS9                   Off 0.13 0.26 0.48 0.00 
     TwoStoneSP50                    On  0.58 0.51 0.61 0.50 0.65 
     Flanger                         Off 0.41 0.66 0.66 
     DelayMono                       Off 0.09 0.17 0.51 0.60 1.00 
     bias.reverb                     On  0.20 0.90 0.61 0.23 0.58 0.65 0.20 1.00 
Message:  301  Send / receive new preset: 1
0 1 AAAAAAAA : 2-Crunch
     bias.noisegate                  On  0.12 0.06 1.00 
     Compressor                      On  0.30 0.52 
     DistortionTS9                   Off 0.13 0.26 0.48 0.00 
     Twin                            On  0.70 0.37 0.45 0.55 0.80 
     Flanger                         Off 0.41 0.66 0.66 
     DelayMono                       Off 0.09 0.17 0.51 0.60 1.00 
     bias.reverb                     On  0.20 0.90 0.61 0.23 0.58 0.65 0.20 1.00 
Message:  301  Send / receive new preset: 2
0 2 Hey Jimi Solo : 1-Clean
     bias.noisegate                  On  0.12 0.33 1.00 
     Compressor                      Off 0.44 0.29 
     JH.FuzzTone                     On  0.86 0.29 
     GK800                           On  0.91 0.42 0.37 0.59 0.89 
     GuitarEQ6                       On  0.45 0.50 0.50 0.51 0.57 0.49 0.45 
     VintageDelay                    Off 0.40 0.65 0.71 1.00 
     bias.reverb                     On  0.28 0.15 0.50 0.62 0.37 0.35 0.10 1.00 
Message:  301  Send / receive new preset: 3
0 3 Preset Test4 : DDDDDDDD
     bias.noisegate                  On  0.21 0.57 0.00 
     Compressor                      On  0.17 0.54 
     DistortionTS9                   Off 0.70 0.28 0.69 
     ADClean                         On  0.68 0.50 0.38 0.59 0.81 
     ChorusAnalog                    On  0.52 0.40 0.24 0.74 
     DelayMono                       On  0.17 0.23 0.49 0.60 1.00 
     bias.reverb                     On  0.69 0.39 0.46 0.69 0.49 0.47 0.30 
Message:  301  Send / receive new preset: 5
1 0 Preset Test2 : BBBBBBB
     bias.noisegate                  Off 0.10 0.57 0.00 
     BlueComp                        Off 0.43 0.66 0.36 0.56 
     DistortionTS9                   Off 0.06 0.74 0.60 0.00 
     Twin                            On  0.61 0.37 0.45 0.55 0.80 
     Flanger                         Off 0.41 0.66 0.66 
     DelayRe201                      On  0.10 0.31 0.49 0.37 1.00 
     bias.reverb                     On  0.56 0.51 0.42 0.30 0.60 0.59 0.00 1.00 

```

Then connect your app (android or ios)

```
Message:  22F  Unprocessed in sync: 22f
Message:  32F  Unprocessed in sync: 32f
Message:  22A  Unprocessed in sync: 22a
Message:  32A  Unprocessed in sync: 32a
Message:  210  Unprocessed in sync: 210
Message:  310  Hardware preset is: 3
Message:  201  Unprocessed in sync: 201
Message:  301  Send / receive new preset: 5
1 0 Preset Test1 : AAAAAA
     bias.noisegate                  Off 0.14 0.22 0.00 
     LA2AComp                        On  0.00 0.85 0.37 
     Booster                         Off 0.72 
     RolandJC120                     On  0.63 0.28 0.16 0.67 0.81 
     Cloner                          On  0.20 0.00 
     VintageDelay                    Off 0.38 0.43 0.42 1.00 
     bias.reverb                     On  0.29 0.41 0.29 0.39 0.58 0.65 0.20 1.00 
Message:  223  Unprocessed in sync: 223
Message:  323  Unprocessed in sync: 323
Message:  170  Unprocessed in sync: 170
Message:  470  Unprocessed in sync: 470
```


Then......every minute it will update the amp and UI with a new preset. You can turn the dials on amp or app and it should work.   

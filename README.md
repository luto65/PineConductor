# Pine Conductor
This repository contains all my source code, ideas and results, for a low cost wearable device supporting the interests and needs of musicians in general and conductors specifically.

## Needs addressed by the Pine Conductor in current version 2.

1. Any musician looks with much love to her / his music instrument. Therefore i thought that a watch face that can be recognized by any musician could be something that would add an additional conductor touch to the device, going away from the traditional analogic or digital watches and instead fostering musicality.

2. Keep the time
It it s quite normal, specially for beginner conductants, of not keeping the time. However it could also be a requirement of a piece of getting it a bit faster or slower, and then get back to the initial time. For these reasor these needs a vibrational metronome is what was needed, simple to use and without un-needed frills.

2. Transpose
Some instruments are teached memorizing a fingering that reflect the construction setting instead of the acoustic setting. This is my own definition for "transposing instruments". For example:

|  Instrument     | Transposition |
| --------------- |  ------ |
| Tenor Saxophone | Bb |
| Alto  Saxophone | Eb |
| French Horn     | F  | 

While with some training it becomes easy to remember (for example, a "natural C" is actually a "D" for a Tenor Saxophonist ) sometime it is not so easy, and often one relies on transposition tables printed on paper and given to the musicians or pasted into the partiture.

3. Gregorian modes
The 7-tone progression (also called scales) across the 12-semitones of the tempereed music system have different sound characteristics depending on the intervals that are used.
In the following table two common modi are described, with the number of semitones from the ground tone to the next in the progression.

| Modi  | 1st | 2nd | 3rd | 4th | 5th | 6th | 7th |
| ----- |  ---- |  ---- |  ---- |  ---- |  ---- |  ---- |  ---- |  
| Major |  2  |  2  | 1   |  2  | 2   | 2  | 1  |
| Minor |  2  |  1  | 2   |  2  | 2   | 2  | 1  |

In classical music, however, there are more than two, actually there are countless, however the more common are:

Lycrian, Ionic (Major), Mixolyidian, Dorian, Aeolic (Minor), Phrygian, Locryan

While mnemonics are available, it could be challenging to spontaneously play in one or the other mode without relying on additional supplemental support (internet, paper, apps). 
## Hardware
### Apple Watch
My first choice was to try the apple Watch, given that I am an Apple-fanatic. My attempts, with various versions of Apple Watch, confirmed what i read in review articles, and namely that the motor used for generating vibration in Apple is too lite for my purposes. While i used old version of Apple Watch (version 4 i think), newer versions did not improve on that aspect, and eventually if they would be improved, they would be beyond my budget.
### Soundbrenner Pulse 112€
I then purchased the [Soundbrenner Pulse, version 1](https://www.amazon.de/gp/product/B01AQNIOO4), for about 112 and it was too strong for my purposes. For the version i purchased, there was no possibility to control the strength of the vibration, and even purchasing the version Core II have been told that no changes were done in that part of the hardware and software control, and anyhow it was going to be beyond my target price.
### PineTime 38€
Finally i landed on PineTime. For just about 30€ i purchased it at the [international site](https://pine64.com/product/pinetime-smartwatch-sealed/), and received it at home in few weeks. Without need of wiring ot any electronics i could immediately develop with it and achieve my purposes.

## Deployment on your PineTime
To facilitate use, i included here a DFU build, so that users with sealed PineTime could simply deploy it as new firmware.
Simply download the release and upload it in your PineTime. For that purpose I use Watchmate since I run OpenSuse Linux in a VMWare hosted from my old MacBook Pro.

## Solutions provided by PineConductor for such needs

### [WatchFace::Conductor](/WatchFaceConductor.md)
![The new Conductor face](https://lucatoldo.de/pineconductor/img/WatchFaceConductor_1440.jpeg)


### Apps
![The Musician Apps](https://lucatoldo.de/pineconductor/img/PineConductorApps.jpg)

### [App::Pulser](/AppPulser.md)
![The Pulser, the first app i develped for musicians on the PineTime](https://lucatoldo.de/pineconductor/img/Pulser-default.jpeg)

### [App::Transposer](/AppTransposer.md)
![The Transposer](https://lucatoldo.de/pineconductor/img/TransposerApp-default.jpeg)

### [App::GregorianModes](/AppGregorianModes.md)
![The Gregorian Modes](https://lucatoldo.de/pineconductor/img/Gregorian-default.jpeg)


## Changes in Version 2
1. WatchFace Conductor
The first version of Pine Conductor had a very artistic representation of the Circle of Fifth, gregorian modes and parallel scales, it was of no practical use due to the small size of the watch. Additionally, it was consuming lots of memory that prevented new functionalities to be added. For this purpose i replaced it with a smarter watch, with unique look, that stimulate thinking in semitones, but is as well appealing to the eyes.

2. new apps
With version 2, the Transposer and the GregorianModes are deployed for the first time.

# Developer notes
In the spirit of the Pine64, i decided to share the code and my knowledge since this could be useful for others too.
In the src you will find all files that I have modified in order to get the PineConductor to work, as well as shell command to facilitate the build.

## VMWare prebuilt IMAGE - available on request (18GB)
One of the complexities in developing for Pine64 is the setup of all the dependencies. As Mac user I first tried to build on my bare metal, however i faced many difficulties. Following the advice of [Unixb0y](https://github.com/unixb0y) i then moved to VirtualBox 7.0.20 with extensions, and purchased the [USB-BlueTooth Dongle RENGOGA 5.4](https://www.amazon.de/dp/B0D9845HFD), with the Suse Linux [Tumumbleweed](https://get.opensuse.org/tumbleweed/) distribution.
With such a setup, running on my MacBook Pro (retina-2013), i am now able to build and upload the firmare directly from the Mac to the PineTime. Unfortunately it is not possible to store VDI (18GB) in github, however if you have interest you can contact me and i will share it via wiretransfer or other ways.


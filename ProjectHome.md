# gt-jtx #

## What is gt-jtx? ##
I feel it's cool to control an airplane with a 3d joystick rather than gimbals. It's intuitive and real aircraft pilots (or those trained on MSFS) can appreciate. See [Yoke](http://en.wikipedia.org/wiki/Yoke_(aircraft)) and [HOTAS](http://en.wikipedia.org/wiki/HOTAS) ("Hotass")

After a good amount of self provocation, I embarked on the journey of doing a joystick based 4 analog and 1 digital channels transmitter for controlling Airplanes. Additionally, this system needs to have a basic menu system, settings (Expo/Dr, Rev etc..) and a monitor

So here comes the gt-jtx which is a work in progress.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=7jzh2vNpVi8' target='_blank'><img src='http://img.youtube.com/vi/7jzh2vNpVi8/0.jpg' width='425' height=344 /></a>

## How does it work? ##
The gt-jtx uses a 3d joystick for controlling an RC airplane. The X axis map to ailerons (roll), Y to Elevator (pitch) and the joystick grip twist/Z axis map to Rudder (Yaw) control. The throttle control has a separate pot on the base of the joystick.
Digital push buttons 5-12 on the joystick base are trim switches for the four channels and pressing unique combos on these switches along with power switch navigates through the menu and settings screen.

As of now the microcontroller (ATMEGA 8) generates a PPM stream based on the inputs it receives from the pots. This input can be fed to your choice of
radio module or to the trainer port of your Tx. My setup is currently hooked up to an FrSky Tx Module. While on the home screen, press menu and you will be taken to the menu system navigable and settings changed via the trim keys

Everything on the menu system and other items are shown on a LCD display mounted to the joystick.

## How can I try it? ##
### The m8 version ###
m8 is a basic version with support upto 4 analog and 1 digital channel.

You can download the zip files from the downloads section and simply arrange the following stuff and flash the firmware to the ATMEGA chip. For having this setup, you'd need:

  1. A **3D joystick** - I use Genius Metalstrike 3D. The price and quality are good and there's quite some space inside. [Get one here](http://www.flipkart.com/genius-metalstrike-3d-joystick/p/itmdf994dawzzzyh?pid=ACCDF992GZN8QYBZ&ref=23a3cf18-9ac7-4138-94d8-e0f05140340d&srno=s_4&otracker=from-search&query=genius)
  1. A Breakout board to fit the joystick - The brains of the joystick are on the same circuit board as the trim switches. We will need to rip out the brain and create our own board using the same micro push switches.  [Hardware Schematic](http://gt-jtx.googlecode.com/files/gt-jtx-breakout.sch) [Board Layout](http://gt-jtx.googlecode.com/files/gt-jtx-breakout.brd). Right Click > Save Link as... (This breakout board depends on the joystick used so if you plan to use a different joystick, this download might be of little use.
  1. The gt-jtx board fabricated. This is simple if you know PCB etching, if not, its fun learning. [Hardware Schematic](http://gt-jtx.googlecode.com/files/gt-jtx-ver1.0.sch) [Board Layout](http://gt-jtx.googlecode.com/files/gt-jtx-ver1.0.brd). Right Click > Save Link as...
  1. An ATMEGA 8, An LED, A 10K resistor, A 10K variable POT, A 16x2 LCD screen, Berg strips, one M/F pair is enough, Loads of wires for routing [Get Everything Here](http://www.lootag.com/store)
  1. A USBASP Atmel programmer. If you have one, good; If not you can send over your chip, I will have it flashed. [Usbasp](http://www.fischl.de/usbasp/)
  1. A radio module. I use Fr-Sky (bought from hobbyking) but you can use any as long as it has PPM input. [Get one here](http://www.hobbyking.com/hobbyking/store/__14353__FrSky_V8HT_2_4Ghz_DIY_Module.html)
  1. A Character LCD Module (16x2)

### The m32 version ###
m32 is an advanced version with support up to 16 channel. Use POTs where you need analog channels and use toggle switches where digital channels are required

The m32 has twp parts, the brain, and the display. The brain always remains the same, but the display can change between a simple GLCD based display or a complex LCD TV based display using the RaspberryPi

  1. A **3D joystick** - I used Thrustmaster HOTAS 3D for this advanced version.
  1. The gt-jtx-m32 board fabricated. This is simple if you know PCB etching, if not, its fun learning. [to schematics etc coming soon #](Links.md)
  1. An ATMEGA 32, An LED, A 10K resistor, Berg strips, one M/F pair is enough, Loads of wires for routing [Get Everything Here](http://www.lootag.com/store)
  1. A USBASP Atmel programmer. If you have one, good; If not you can send over your chip, I will have it flashed. [Usbasp](http://www.fischl.de/usbasp/)
  1. A radio module. I use Fr-Sky (bought from hobbyking) but you can use any as long as it has PPM input. [Get one here](http://www.hobbyking.com/hobbyking/store/__14353__FrSky_V8HT_2_4Ghz_DIY_Module.html)
  1. A RaspberryPi
  1. A LCD TV (choose your own size, anything from 4 inches to 46 inches works.
  1. The following firmwares/softwares. You will need the brain firmware irrespective for what display you use
    * Brain Firmware **_Links coming soon_**
    * Simple Display Firmware (Displays on GLCD) **_Links comng soon_**
    * [Complex Display Firmware (Displays on your LCD TV screen using OpenGLES on Raspberry Pi) - ARTRIX](http://code.google.com/p/artrix/)


## How much will the build cost? ##

The cost for the m8 system was about INR 6000 including the FrSky Tx module. Sky is the limit for putting in whatever you want. As of now I feel it's
a little on the higher side as it is a prototype. I have no plans to go into production, but if you really like what I've done and are too lazy to do the same
I wouldn't mind replicating the unit.

The cost for the m32 system was about INR 12000, everything included.

## What's next? ##
Ultimately, I am going to beef up the micro controller on this and write down some USB firmware and
have a USB port to which standard USB joysticks maybe connected.
Another update I have in mind is to use Hall sensors for motion detection instead of the regular pots in the joystick.
A Futaba NR-4J pack fits very well inside. Alternatively use a 2s Lipo with a voltage reg, fits well.


### Disclaimer ###

I take no responsibility or warranty for any damage to you or your equipment, that may arise out of or is a consequence of using or reproducing any parts or features of the gt-jtx software/hardware system. Everything you do is completely at your own risk and expense.



---


By downloading any parts of this software or reproducing any part of the hardware system you are agreeing to the terms specified in the [Licensing](http://code.google.com/p/gt-jtx/wiki/Licensing) and the spirit of thereof.
# gt-jtx
What is gt-jtx?
I feel it's cool to control an airplane with a 3d joystick rather than gimbals. It's intuitive and real aircraft pilots (or those trained on MSFS) can appreciate. See Yoke and HOTAS ("Hotass")

After a good amount of self provocation, I embarked on the journey of doing a joystick based 4 analog and 1 digital channels transmitter for controlling Airplanes. Additionally, this system needs to have a basic menu system, settings (Expo/Dr, Rev etc..) and a monitor

# How does it work?
The gt-jtx uses a 3d joystick for controlling an RC airplane. The X axis map to ailerons (roll), Y to Elevator (pitch) and the joystick grip twist/Z axis map to Rudder (Yaw) control. The throttle control has a separate pot on the base of the joystick. Digital push buttons 5-12 on the joystick base are trim switches for the four channels and pressing unique combos on these switches along with power switch navigates through the menu and settings screen.

# The ATMEGA 8 Version

m8 is a basic version with support upto 4 analog and 1 digital channel.

As of now the microcontroller (ATMEGA 8) generates a PPM stream based on the inputs it receives from the pots. This input can be fed to your choice of radio module or to the trainer port of your Tx. My setup is currently hooked up to an FrSky Tx Module. While on the home screen, press menu and you will be taken to the menu system navigable and settings changed via the trim keys

Everything on the menu system and other items are shown on a LCD display mounted to the joystick.

You can clone the repo, build and flash the firmware to the ATMEGA chip. For having this setup, you'd need:

A 3D joystick - I use Genius Metalstrike 3D. The price and quality are good and there's quite some space inside. Get one here
A Breakout board to fit the joystick - The brains of the joystick are on the same circuit board as the trim switches. We will need to rip out the brain and create our own board using the same micro push switches. Hardware Schematic Board Layout. Right Click > Save Link as... (This breakout board depends on the joystick used so if you plan to use a different joystick, this download might be of little use.
The gt-jtx board fabricated. This is simple if you know PCB etching, if not, its fun learning. Hardware Schematic Board Layout. Right Click > Save Link as...
An ATMEGA 8, An LED, A 10K resistor, A 10K variable POT, A 16x2 LCD screen, Berg strips, one M/F pair is enough, Loads of wires for routing Get Everything Here
A USBASP Atmel programmer. If you have one, good; If not you can send over your chip, I will have it flashed. Usbasp
A radio module. I use Fr-Sky (bought from hobbyking) but you can use any as long as it has PPM input. Get one here
A Character LCD Module (16x2)

# The ATMEGA32 version

m32 is an advanced version with two parts, the brain and the controller. 
The brain is reponsible for the ppm signal generations, whereas the controller takes care of the 'pn ground' activities such as model management, pilot settings etc

A 3D joystick - I used Thrustmaster HOTAS 3D for this advanced version.
The gt-jtx-m32 board fabricated. This is simple if you know PCB etching, if not, its fun learning. Schematics in the hardware section

- An ATMEGA 32, An LED, A 10K resistor, Berg strips, one M/F pair is enough, Loads of wires for routing.
- A USBASP Atmel programmer. If you have one, good; If not you can send over your chip, I will have it flashed. Usbasp
- A radio module. I use Fr-Sky (bought from hobbyking) but you can use any as long as it has PPM input. 
- An Android device with USB OTG support
- gt-jtx m32 programmed onto the ATMEGA32 board
- Artrix App loaded on the Android device


# Disclaimer
I take no responsibility or warranty for any damage to you or your equipment, that may arise out of or is a consequence of using or reproducing any parts or features of the gt-jtx software/hardware system. Everything you do is completely at your own risk and expense.

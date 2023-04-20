# Extra matherials

There ara some additional informations and sousces are present here

## I2S test audio_recorder_objif_mod

This test baset on default I2S test placed on spresense/examples/audio_recorder_objif
There are two files infide:
    audio_recorder_objif_main.cxx - source files
    nuttx.spk - compiled firmware

Setup Spresense SDK and firmwaring are described here:
https://developer.sony.com/develop/spresense/docs/sdk_set_up_en.html


### Prepearind and runing the example

The solution is to call board_audio_i2s_enable() for setting the pin configuration on the application side.

Steps:
 
Run MSYS2 MSYS

Run this command to activate installed tools.

source ~/spresenseenv/setup

Open SDK folder:

cd spresense/sdk

On sdk directory, execute below:
./tools/config.py examples/audio_recorder_objif

You need to modify 3 things after that:
1.      I2S selection change on configuration.
2.      I2S master or slave selection change on configuration.
3.      sample code modify to use I2S not microphone.
 
I will describe them below. You may use edited source file instead editing.
 
1.  Modify configuration for selecting I2S0 and master mode.
To configure them please execute the below command on sdk directory.
 
 make menuconfig
 
Then menuconfig is appeared  on your terminal,
Got to Board Selection -> Audio I2S settings
Then you can select I2S0.
 
 
2. Modify the selection of master or slave.
In the menuconfig, go to Board Selection -> Audio baseband config settings -> I2S settings -> I2S0 device mode selection.

Then you need to select  Master mode.
 
 
After finishing those 2 configurations. Go out the menuconfig with saving it.
 
3.  Modify source code for using I2S input.
 
Open examples/audio_recorder_objif/audio_recorder_objif_main.cxx
Modify line 567 from

act_param.param.input_device = AS_SETRECDR_STS_INPUTDEVICE_MIC;

to

act_param.param.input_device = AS_SETRECDR_STS_INPUTDEVICE_I2S;
 
And modify line 583 from

act_param.param.input_device = AS_SETRECDR_STS_INPUTDEVICE_MIC;

to

act_param.param.input_device = AS_SETRECDR_STS_INPUTDEVICE_I2S;
 
4.  Modify source code for calling board_audio_i2s_enable();
 
Function prototype for board_audio_i2s_enable() to avoid including driver function header.

extern "C" void board_audio_i2s_enable(void);
 
And call it just before the line of "if (!app_start_recorder())".
 
5.      build
 
On the sdk directory, do  make command to build a binary.

make

6. Flash

tools/flash.sh -c /dev/ttyUSB0 nuttx.spk - for Linux

or

tools/flash.sh -c /COM8 nuttx.spk - for Windows

7. Checking

Connect to the board oscilloscope to pins:
J1 Sony board (J3 Sensors board) pin 6 - I2S0_BCK - I2S clock
J1 Sony board (J3 Sensors board) pin 7 - I2S_LRCLK - I2S frame select
J2 Sony board (J4 Sensors board) pin 6 - I2S0_DATA_IN - I2S mic data

Run Windows terminal and connect it to COM port with speed 115200 baud per sec

Send to the device:

audio_recorder_objif

The string should have endline symbols at the end (13, 10 (dec)) some terminals add them automatically in other cases this symbols should be added:

audio_recorder_objif#013#010

After that you should see I2S interfaces pulses

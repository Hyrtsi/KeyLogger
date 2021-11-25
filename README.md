# Keylogger

## Introduction

This is an example of how to read the keyboard and mouse in Windows and playback that later.
Keylogger is a good template for bots and other automatic stuff. For professional use, go and find some other framework.

I made this for purely learning and curiosity purposes to bot rs3 and learn to program stuff that is potentially very useful in the future.

The software works as follows
- Gather user keyboard and mouse inputs and save them to a file
- Afterwards, read the inputs from a file AND repeat them by giving the OS the inputs using Win32Api

#### Here are my mouse movements from the timespan of a few hours of programming and surfing on the blogosphere

![photo5886390530894573425](https://user-images.githubusercontent.com/17768288/143426671-4a982c3a-c2dd-4dbb-aeb7-103b1ef4da31.jpg)

## Dependencies: `Windows`

## TODO

- remove absolute paths from vs files
- the whole vs could be removed from this project
- use Lehdari's pulautin and such machines, cmake
- minimal working demos, preferably in different repo or branch than the main thing that is under construction
- tests maybe
- plug and play examples
- UI with imgui

## Lessons learned

- Win32Api documentation is sometimes very good
- You might want to emulate the keypresses at hardware level
- If you have a certain layout (such as US Dvorak), the key press detecting functions suffer...
- If you are trying to do things in real time you will face problems but it's possible

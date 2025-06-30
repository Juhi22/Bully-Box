# Bully Box
Bully Box is a physical machine that can be used as a present, a super birthday gift... to prank you friend.
This repo is only a place where I tried to collect some old codes to be in one place. Unfortunatelly there is no plans how to build the exact box.

![alt text](https://github.com/Juhi22/Bully-Box/blob/main/bullyBox.jpg?raw=true)

## What can it do?
### Main feature
Mainly this is a fusion of a quiz game and an escape room.
You have got your smart phone and the box. There are questions displayed in your phone with colored answers, to answer you have to press the correct button on the box.
Between several questions there are some pranks (side features). It depends on the configuration in the phone, it controls the steps (`/mobileApp/resources/default/quiz.json`).

### Side features
- Plug in the box to a socket to get electricity, then the game starts
- There is an audio player configured in the box, it always tells you some instructions (or laugh at you)
- There are many numbered doors. The numbers are changeable. These are locked and only opens when "story" is at that point. Behind these doors, we can find the majority of side features
- Send sms to the given person (config file: `/mobileApp/resources/default/smsdata.json`)
- Play led matrix labyrinth to get the pin code of the bluetooth
- Try to beat the limit in the breathalyzer
- Cut the right wire, be careful is it the blue or the red one?
- Move a metal ring without touching the wire
- Fires shaving foam out of two little holes
- Throw out a huge pile of corn. In it there is an RFID tag (use it at the right spot)
- There is a huge pipe in the box it is filled with jelly. At the bottom of it there is a key (use it at the right spot)
- There is a confetti cannon, that shots colorful papers in the air as a celebration (also you can put some kind of drinks next to it as a present)

## Communication
- Internet is not necessary but in a part (sms sending) it is used.
- Between the smartphone and the box the communication is done via bluetooth.
- There are 3 arduino devices in the box: mega (main controller, brain of the box), uno (bluetooth communication), nano (audio player). Between these the comminucation is wired.

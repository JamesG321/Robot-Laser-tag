# Robot-Laser-tag
Robot laser tag project made for Intro to embedded systems course as final project, "make something cool".

I decided to combine some modules that my group written throughout the quarter to create a robot laser tag system.
The player using the robot can control the robot wirelessly via a joystick controller while the attacker has a modified NES light zapper
that "shoots" IR signals to the repurosed IR distance sensors on the robot. the robot loses HP when it is hit and the controler displays the HP and also reacts by vibrating briefly.

Here is a breif [YouTube video demo](https://www.youtube.com/watch?v=cTD_Vb94pVg) of the project.

See my [Lab Report](https://github.com/JamesG321/Robot-Laser-tag/blob/master/Lab5Report%20group%2021.pdf) in the repo for more details of the hardware setup.

## Getting Started

This project is built using 
[Beaglebone black](https://beagleboard.org/black) boards for the robot and the controller.
Being famliar with c and linux and how to connect to a host (the boards) via USB to run code is recommended.

Building a similar system using different parts is possible, but adjusting the GPIOs and board interface variables accordingly is required. 

## Documentation

See [robot laser tag.pdf](https://github.com/JamesG321/Robot-Laser-tag/blob/master/robot%20laser%20tag.pdf) for detailed documentation of setup.

## Built With

[Beaglebone black](https://beagleboard.org/black)

["PS2" Joystick breakout board](https://www.amazon.com/WGCD-Joystick-Breakout-Controller-Arduino/dp/B01N59MK0U/ref=sr_1_1?ie=UTF8&qid=1511179651&sr=8-1&keywords=joystick+breakout+module&dpID=51TYC4fWDkL&preST=_SX300_QL70_&dpSrc=srch)

[LCM1602C 16×2 LCD display](http://www.datasheetcafe.com/lcm1602c-datasheet-pdf/)

[Uxcell DC1.5V/0.04A 3V/0.09A 1000RPM 4mm Micro Vibration Motor](https://www.amazon.com/gp/product/B00PZV52O8/ref=oh_aui_search_detailpage?ie=UTF8&psc=1)

[Toshiba TB6612FNG H-bridge Intergrated Circuit](https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf) 

[Spark Fun Multi-Chassis - Tank Version](https://www.sparkfun.com/products/12091)

## Authors

* **James Guo** - [GitHub](https://github.com/JamesG321)

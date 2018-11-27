## a hybrid HID device for differently abled ##
Submitted as Embedded Systems for Society Lab project in 2015 at VJTI, Mumbai towards the completion of our bachelors degree.

1. The hybrid HID device connects to the computer using USB, and mimics a keyboard, mouse and joystick at the same time. The idea for developing this device was that differently abled individuals who aren't able to use a keyboard and mouse yet, can navigate through Windows and use a computer with almost full functionality. 
2. Speech engine on Windows was (trained and) activated when this device was connected, making the interface interactive, and navigate-able even if the user wasn't able to see the screen.
3. Thanks to [Tilen Majerle](https://github.com/MaJerle)! our projects on STM32F407/STM32F429 were manageable because of his layer of abstraction given over the HAL/STD Peripheral library by STM. In case of absence of headers while compiling do check out his [libraries](https://stm32f4-discovery.net/). 
4. Documentation in the PDF file [here](https://github.com/nilay994/hybridHID/tree/master/hybridHID.pdf)

The picture below shows the device in its final stages of submission: The tiny board sat on top of an haptic feedback ring, 
![HIDring](https://github.com/nilay994/hybridHID/tree/master/ring.jpg)


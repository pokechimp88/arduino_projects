# Arduino Projects
Code and schematics for various Arduino projects that I've made over the years.

#Moscow Metro Em-509 subway train control panel (mosmetro_controlpanel_em509)

This project requires Arduino HID library or firmware (through Atmel FLIP) installed to work properly. I have not tested it due to lack of resources.

This is a realistic replica of the control panel of the Em-509 subway trains formerly used on the Moscow Metro system that functions as an HID-compatible keyboard, allowing you to play train simulators such as GMod Metrostroi more immersively.
The Arduino board receives analogue inputs from the throttle lever, brake valve, reverser switch, and emergency control system, as well as digital inputs from various buttons on the panel, as well as the vigilance pedal and horn pedal. (some extra buttons that are not pressed together are handled through analog input to free up pins).
The serial output pins (UART) are used to drive a shift register that is connected to a pair of LED 7-segment digits, displaying the train's speed from 0 to 99 kilometers per hour. A few other digital pins handle the LEDs for the Green RP lamp (overload relay), red RP/LSN lamp (engine failure), and door indicator.
The code for handling the speedometer and indicators is not yet complete as I need to figure out a way to pass values of in-game variables to the Arduino controller.

#Serial Announcement System (uart_asnp)

This program uses the library for the Open-Smart Red MP3 player module to play back Montreal Metro announcements from a microSD card, and allows the operator to select the start station and direction, as well as allowing to skip stations. Future additions may include the ability to select different lines, if program memory permits.

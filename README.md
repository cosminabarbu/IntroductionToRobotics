`# IntroductionToRobotics ( 2023 - 2024 )


The Robotics laboratory assignments are part of the curriculum for third-year students at the Faculty of Mathematics and Computer Science, University of Bucharest. Each assignment encompasses specified criteria, detailed implementation instructions, as well as accompanying code and image files.

# Homework no. 1 

This task emphasizes the management of each channel (Red, Green, and Blue) of an RGB LED through three separate potentiometers according to the following setup: 

Link to the setup photo: https://github.com/cosminabarbu/IntroductionToRobotics/blob/main/setups/setup1.jpg

Can also be seen at: https://www.youtube.com/watch?v=sFL1ZQPevVA .


# Homework no. 2

For this task I will be implementing a 3-floor elevator simulator using Arduino and a few components, such as LEDs, buttons and a buzzer. 
Taking the process step by step, I began by placing 3 buttons on a breadboard which I then connected to 3 LEDs, one LED for each button. Each LED has also its own resistor (I used the 330 Ω ones) that is connected to GND on the breadboard. I used a fourth LED and implemented it so that when the elevator is moving it is blinking, then when the elevator is in idle state it stays on. The last step was adding the buzzer, which makes different-toned noises when the elevator leaves a floor or arrives at a floor. 

Link to the setup photo: https://github.com/cosminabarbu/IntroductionToRobotics/blob/main/setups/setup2.jpg


Youtube link: https://youtu.be/r8Axozp_nhE .


# Homework no. 3

This task consists of using a 7-segment display and "drawing" on it by using a joystick to move through the component's segments and making them light. For each segment of the display I used a resistor (220Ω or 330Ω) which I then connected to the arduino pins as seen in the following scheme.
![image](https://github.com/cosminabarbu/IntroductionToRobotics/assets/93591428/20501e15-6f65-4977-972e-3a4cd4d9adfc)

Lastly, I connected the joystick to the arduino and got to work.

Link to the setup photo: https://github.com/cosminabarbu/IntroductionToRobotics/blob/main/setups/setup3.jpeg

Youtube link: https://youtu.be/p8XjfkXcT8g?si=ZGsuWSaOdnA6sdmf

# Homework no. 4

This task is something close to a big brother of the last one. For this homework I will be implementing a 4 digit 7 segment display to act like a stopwatch timer using a SR (Shift Register) for multiplexing. To start with, the hardest part of getting the setup done was dealing with the resistors for the display and the cables (it can be seen that cable management is not in its greatest form). Afterwards, I added the push-buttons on another breadboard and connected them to ground and the arduino. Each button has its own task: the 1st button is the reset button, the 2nd button is called start/stop button and the 3rd one is for laps. Each one works as following:
 <a href="r"> - the reset button should set all digits to 0 only when the timer is not counting and also, when pressed, it should delete all of the laps saved prior. </a>
  < a href="ss"> - the start/stop button does what its own name says: if it's pressed when the timer is in idle mode, it will start counting and if pressed when the timer is counting, it will pause the counting process and display the number of that current moment. </a>
 <a href="l"> - the laps button can save laps when the timer is running or cycle through the saved laps once the timer is paused. </a>

Link to the setup photo: https://github.com/cosminabarbu/IntroductionToRobotics/blob/main/setups/setup4.jpg

Youtube link: https://youtu.be/eY0EQmNK5Sc


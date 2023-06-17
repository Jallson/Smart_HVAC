## Smart HVAC System with Sony Spresense by Jallson Suryo

Demo Video link: https://youtu.be/LLYOtY7svDQ

![image17.jpg](https://www.dropbox.com/scl/fi/zjmzecrvdzyvs4coel906/image17.jpg?dl=0&rlkey=gzgwwv9d2lu9jdp4yd7vgupno)
Smart HVAC with Sony Spresense camera - PoC Demo setup

This project is a modified project from the previous project with the same title using the Arduino Nicla Vision.
Then all the references and steps can be seen directly taken from Smart HVAC System with Nicla Vision project:(https://github.com/Jallson/Smart_HVAC/blob/main/HVAC_Smart_System.md))

Edge Impulse project link: https://studio.edgeimpulse.com/public/215243/latest

![image18.png](https://www.dropbox.com/scl/fi/5cc8p4cve9wuynuo6uuot/image18.png?dl=0&rlkey=2h0prxx4mrl4b5jcs2dj5r14r)
Diagram Smart HVAC with Sony Spresense expansion board with Camera

All the steps from 1 to 5 can be followed from provious project by just skipping part 4; Sony Spresense doesn't use Open MV IDE. Whereas in stage 5 Deploying the program via the Arduino IDE, we have modified the code, which can be downloaded here (https://github.com/Jallson/Smart_HVAC/blob/main/spresense_camera_smartHVAC_oled.ino) and the code for the Arduino Nano peripheral *Nano_SmartHVAC_I2C_Peripheral.ino* can be downloaded here (https://github.com/Jallson/Smart_HVAC/blob/main/Nano_SmartHVAC_I2C_Peripheral.ino).

By successfully using Sony Spresense in this Smart HVAC project, it can be concluded that the flexibility of the ML model by Edge Impulse can be directly applied to different types of Micro-controllers with only minimal modification changes. And this Sony Spresense board has potential with more RAM and a larger Sony camera sensor than Nicla Vision.

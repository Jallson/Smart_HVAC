## Smart HVAC System with Sony Spresense by Jallson Suryo

This is an alternative version of the Smart HVAC project using the Sony Spresense development board. The Nicla Vision version of this project can be found here: (https://github.com/Jallson/Smart_HVAC/blob/main/HVAC_Smart_System.md)

Demo Video link: https://youtu.be/LLYOtY7svDQ
Edge Impulse project link: https://studio.edgeimpulse.com/public/215243/latest

![image17.jpg](https://www.dropbox.com/scl/fi/zjmzecrvdzyvs4coel906/image17.jpg?dl=0&rlkey=gzgwwv9d2lu9jdp4yd7vgupno)
Smart HVAC with Sony Spresense camera - PoC Demo setup
![image20.jpg](https://www.dropbox.com/scl/fi/hgckzd1sxyp0ypvyi6ir5/image20.jpg?dl=0&rlkey=e72z6dxmsgigb580eq7ed6lvy)
![image21.jpg](https://www.dropbox.com/scl/fi/h2jkyuwg3poc38achav3f/image21.jpg?dl=0&rlkey=009vygt9s8af71pydn2tzd74w)

![image18.png](https://www.dropbox.com/scl/fi/5cc8p4cve9wuynuo6uuot/image18.png?dl=0&rlkey=2h0prxx4mrl4b5jcs2dj5r14r)
Diagram Smart HVAC with Sony Spresense expansion board with Camera

All the steps from 1 to 5 (skipping part 4) is the same with the previous project; Sony Spresense does not use the Open MV IDE hence part 4 can be skipped. In part 5, Deploying the program via the Arduino IDE, we have modified the code, which can be downloaded here (https://github.com/Jallson/Smart_HVAC/blob/main/spresense_camera_smartHVAC_oled.ino) and the code for the Arduino Nano peripheral *Nano_SmartHVAC_I2C_Peripheral.ino* can be downloaded here (https://github.com/Jallson/Smart_HVAC/blob/main/Nano_SmartHVAC_I2C_Peripheral.ino).
![image19.png](https://www.dropbox.com/scl/fi/jeuplqu35yxeib3jqc7of/image19.png?dl=0&rlkey=ii8t4c2ngdbar6u36hti853oi)

### Conclusion
By using Sony Spresense, this Smart HVAC project successfully demonstrates that the flexibility of the ML model using Edge Impulse can be directly applied to different types of Micro-controllers with only minimal modification needed. This Sony Spresense development board has the potential with more RAM and a larger Sony camera sensor than the Nicla Vision.

Additional case design for Sony Spresense for this project:
![image22.png](https://www.dropbox.com/scl/fi/1tts426b5adlrch9ciasj/image22.png?dl=0&rlkey=msit1dr4ao4exrmj6z2selm5k)
![image23.png](https://www.dropbox.com/scl/fi/kibjp1tdw5zbi8usyrd3e/image23.png?dl=0&rlkey=evsmzln7ef2ztb4oaohuuko74)

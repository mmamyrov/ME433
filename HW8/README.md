# I2C IO Expander

I think I got a faulty MCP23017 IO Expander. I've tried debugging it in many ways, but still unable to consistently run the program for I2C. 
It reads and sets the IO expander registers in the first second or so (see demo and images for LED working with button), but then hangs in one of the i2c functions, 
since the heartbeat disappears. I read that it could be a floating RESET pin, but I connected it to 3.3V, so I don't think that's the issue. Also, the I2C communication with the IMU seems to be working fine as seen in HW11, which makes me think that it's more of a hardware issue with the IO Expander. 

I'm pretty sure my code is correct, so I don't know what to do. Last year you mentioned that it would be okay to just submit the code 
without the hardware cause of travel. So if the code is correct, could this be enough? 

### IO Expander LED Light On
![Yellow LED is ON when IO Expander Button is Pressed](https://github.com/mmamyrov/ME433/blob/master/HW8/i2c%20Circuit%20On.jpeg)

### IO Expander LED Light Off
![Yellow led is off when io expander button is not pressed](https://github.com/mmamyrov/ME433/blob/master/HW8/i2c%20Circuit%20Off.jpeg)

### IO Expander Demo
[Click Here](https://youtube.com/shorts/wFuQEU7iYzI?feature=share) for the demo recording on Youtube. The demo file is also included in this folder


## Fire Engine Control System

This project involves the development of a fire engine control system using various hardware and software components. The system is designed to detect the location of a fire using OpenCV and a Go Pro camera, and extinguish it using a water pump actuated by a servo motor. The fire engine is operated using a Sony PlayStation 4 remote, and the communication between the components is achieved using Nu-LB-NUC140 and HC-05 with UART communication. The L298N is used to control the wheels using PWM for speed.

### Hardware Components

-   Go Pro Camera
-   Sony PlayStation 4 Remote
-   Nu-LB-NUC140
-   HC-05 Bluetooth Module
-   L298N Motor Driver
-   Servo Motor
-   Water Pump
-   Relay

### Software Components

-   OpenCV
-   Python
-   C

### System Architecture

![System Architecture](https://github.com/RonBenDavid/Fire-Engine-Control-System-with-Remote-and-Computer-Vision-Nu-LB-NUC140/blob/main/IMAGES/firetruck.drawio.png?raw=true)

The Go Pro camera is used to capture live video of the surrounding area, and the video frames are processed using OpenCV to detect the location of the fire. The Sony PlayStation 4 remote is used to operate the fire engine, and the commands are transmitted wirelessly using the HC-05 Bluetooth module. The Nu-LB-NUC140 is used as the main control unit, which receives the commands from the remote and controls the L298N motor driver to move the fire engine in the desired direction. The servo motor is used to actuate the water pump, which is used to extinguish the fire.

### Implementation Details

The implementation of this project involved the development of various components, including the hardware setup, software programming, and integration of the components.

#### OpenCV Fire Detection

![video](https://github.com/RonBenDavid/Fire-Engine-Control-System-with-Remote-and-Computer-Vision-Nu-LB-NUC140/blob/main/IMAGES/gif4.gif)

The OpenCV component is used to detect the location of the fire in the video frames captured by the Go Pro camera. This is achieved by converting the video frames to the HSV color space and applying a red color range mask to isolate the red objects in the frame. The mask is then used to find the contours of the red objects, which are filtered to remove small contours. A bounding box is then drawn around the remaining contours, and the position data is printed to the console and written to a file named "stepper.txt". The position data is used by the servo motor to move the water pump towards the fire source for extinguishing.

#### Sony PlayStation 4 Remote Control

![video](https://github.com/RonBenDavid/Fire-Engine-Control-System-with-Remote-and-Computer-Vision-Nu-LB-NUC140/blob/main/IMAGES/gif2.gif)

The Sony PlayStation 4 remote is used to operate the fire engine by sending wireless commands to the Nu-LB-NUC140 using the HC-05 Bluetooth module. The Pygame library is used to initialize a window and allow the user to control the device using the remote. The program sets up a Bluetooth connection with the device and reads button and analog values from the controller. The program then sends data to the device over Bluetooth based on the current values of the variables.

#### L298N Motor Driver Control

The L298N motor driver is used to control the wheels of the fire engine using PWM for speed. The Nu-LB-NUC140 receives the commands from the Sony PlayStation 4 remote and controls the L298N motor driver to move the fire engine in the desired direction.

#### Servo Motor Control

The servo motor is used to actuate the water pump, which is used to extinguish the fire. The position data of the fire location is obtained from the OpenCV component, and the servo motor is moved accordingly to direct the water pump towards the fire source.

### Conclusion

![video](https://github.com/RonBenDavid/Fire-Engine-Control-System-with-Remote-and-Computer-Vision-Nu-LB-NUC140/blob/main/IMAGES/gif.gif)

The fire engine control system is a complex project that involves the integration of various hardware and software components. The system is designed to detect the location of a fire using OpenCV and a Go Pro camera and extinguish it using a water pump actuated by a servo motor. The fire engine is operated using a Sony PlayStation 4 remote, and the communication between the components is achieved using Nu-LB-NUC140 and HC-05 with UART communication.

The implementation of this project required significant knowledge and expertise in areas such as hardware and software programming, image processing, and control systems. The use of OpenCV allowed for accurate fire detection, while the Sony PlayStation 4 remote provided a user-friendly interface for controlling the fire engine. The integration of the different components was achieved through careful planning and programming to ensure the system worked seamlessly.

Overall, this project demonstrates the potential for technology to be used in innovative ways to address real-world problems. The fire engine control system has the potential to assist firefighters in tackling fires more efficiently and safely, reducing the risk to human life and property.

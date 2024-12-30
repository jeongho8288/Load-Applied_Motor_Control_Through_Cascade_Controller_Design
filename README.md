# Project conducted in Robotics Laboratory 4

A cascade-structured controller was designed to control a motor under load.  
(position-velocity-Torque controller)  
The designed controller was implemented on the Atmega128 microcontroller to control the motor.  
In Visual Studio 2017, a user interface (UI) was designed, and ODE simulations were applied.  
Finally, the controller was deployed on the Atmega128, and motor control was achieved through communication between Atmel Studio and Visual Studio.  

When a desired angle is input, the robotic arm moves clockwise or counterclockwise to reach the specified angle.  
At this time, the angle can be entered in two modes: Joint Mode and Infinite Mode.  
- In Joint Mode, the arm moves only within the range of -180° to 180°.
- In Infinite Mode, the arm rotates continuously to reach the desired angle without any range limitation.  

For example:
- Joint Mode: 320° → -40°
- Infinite Mode: 400° → 400°  
  
### System Architecture
<img src="https://github.com/user-attachments/assets/f71ccd54-e052-4c49-92e8-f7874f00a6e1" alt="Description2" style="width: 40%; height: 300px;">  

### Hardware Images
<div style="display: flex; justify-content: space-around;">
  <img src="https://github.com/user-attachments/assets/9d6a52ba-993f-44bc-ac42-0f91d5d575c0" alt="Description1" style="width: 23%; height: 300px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/040bea4c-3475-4842-9532-60c6ee922d72" alt="Description2" style="width: 23%; height: 300px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/8365030e-4028-4324-a1d5-a86413f4f0c2" alt="Description2" style="width: 40%; height: 300px;">
</div>
<hr style="border-top: 3px solid #bbb;">

## Load inertia
<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/edf8146a-45c5-4cba-8799-0b88b8bb135a" alt="Description1" style="width: 40%; height: 250px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/e4f0d2ac-1bcc-4d2f-aac7-40b35eef1a09" alt="Description" alt="Description2" style="width: 20%; height: 100px;">
</div>
<hr style="border-top: 3px solid #bbb;">

## Cascade Controller
A current controller was designed, and based on its frequency response, a speed controller was subsequently designed.  
Similarly, considering the frequency response of the speed controller, a position controller was designed.  
  
For the position controller, PD control was employed to ensure precision and stability.  
Both the speed and current controllers utilized PI control, and to prevent error accumulation, values were compensated using anti-windup gain.  
  
Saturation limits were set for each controller to ensure the outputs did not exceed predefined thresholds.  
  

- (position-velocity-Torque controller)    
<img src="https://github.com/user-attachments/assets/b905271a-ae75-46ed-a70a-7e286d39e1bf" alt="Description" alt="Description2" style="width: 50%; height: 250px;">

- current controller (PI Control)  
<img src="https://github.com/user-attachments/assets/a2ddba02-634a-4b72-bdf9-844ebdfdbfd0" alt="Description" alt="Description2" style="width: 50%; height: 340px;">

- velocity controller (PI Control)  
<img src="https://github.com/user-attachments/assets/74367bee-66de-4c85-b4d7-b36f4a8a59a2" alt="Description" alt="Description2" style="width: 50%; height: 340px;">

- position controller (PD Control)  
<img src="https://github.com/user-attachments/assets/28e2d020-805c-4d31-87fc-ce5408452f86" alt="Description" alt="Description2" style="width: 50%; height: 250px;">  
<hr style="border-top: 3px solid #bbb;">

## Simulation Results

### UI Interface
<img src="https://github.com/user-attachments/assets/4856096e-f343-4109-8c8d-a4ebfdc7b8a7" alt="Description" alt="Description2" style="width: 35%; height: 300px;">

### Forward Kinematics
- join1=90, joint2=90 (Forward) & join1=-150, joint2=90 (Forward)

<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/7f04d3ae-e375-4fcc-8658-3fc1ebefe3f1" alt="Description1" style="width: 25%; height: 300px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/40308629-b068-4745-b54c-153a5258b18c" alt="Description" alt="Description2" style="width: 25%; height: 300px;">
</div>

### Inverse Kinematics
- x=1.0, y=-1.0 (Inverse)        & x=-1.0, y=0.8 (Inverse)

<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/e3b4127f-e730-46c0-bbaa-aa522fa5b958" alt="Description1" style="width: 25%; height: 300px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/d2f1470f-51b6-4072-8c95-e3da94707ef0" alt="Description" alt="Description2" style="width: 25%; height: 300px;">
</div>
<hr style="border-top: 3px solid #bbb;">


## circuit diagram

<div style="display: flex; justify-content: space-around;">
  <img src="https://github.com/user-attachments/assets/4d8ab495-3d44-472a-8ca2-7db1289ccecf" alt="Description1" style="width: 45%; height: 250px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/73936e29-3da8-41cb-9e17-55d9124fc7e4" alt="Description2" style="width: 45%; height: 250px; margin-right: 2%;">
</div>
<img src="https://github.com/user-attachments/assets/f8a9f11e-201d-4eac-870c-fdd188be039d" alt="Description2" style="width: 45%; height: 250px;">
<hr style="border-top: 3px solid #bbb;">

## Cascade Controller
(position-velocity-Torque controller)  

## Results

<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/c7909233-b382-4147-bc1a-8802bd04d2ca" alt="Description1" style="width: 30%; height: 200px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/c0591b26-37f2-4f97-b80a-e8b3e7575715" alt="Description2" style="width: 30%; height: 200px;">
</div>

<img src="https://github.com/user-attachments/assets/e9cb752c-fcc2-489c-a148-d3ab53af9464" alt="Description1" style="width: 62%; height: 250px;">
<hr style="border-top: 3px solid #bbb;">


## Problems Encountered During the Project

The control cycle of the overall periodic control timer was set to 10ms for sampling. However, when receiving the values via UART0 communication and checking the log files, 
there were slight discrepancies between the number of samples and the elapsed time. This was assumed to be due to the limitations of processing speed in the code and the communication speed.

Additionally, issues such as missing or fluctuating sensor values were resolved by referring to the Atmega128 datasheet to adjust pin configurations and debugging through UART0 communication, making incremental modifications to the code.






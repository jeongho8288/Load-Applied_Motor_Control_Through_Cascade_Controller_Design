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
<img src="https://github.com/user-attachments/assets/f71ccd54-e052-4c49-92e8-f7874f00a6e1" alt="Description2" style="width: 50%; height: 200px;">  

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
<img src="https://github.com/user-attachments/assets/d8d3bbbf-5393-422d-9adc-e709926b01cc" alt="Description" alt="Description2" style="width: 60%; height: 400px;">

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

## Results

- Infinite Mode, +280 degree
<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/cff134b8-51b5-427d-b83e-6b3553db7fe5" alt="Description1" style="width: 50%; height: 250px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/ccce71b2-3e86-4d05-8218-e67d206c3d54" alt="Description2" style="width: 30%; height: 250px;">
</div>  
  
- Infinite Mode, -90 degree  
Due to hardware discrepancies, the system cannot precisely point to -90 degrees.  
<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/ac66b008-a3bf-4bbb-9ab1-02916b19f947" alt="Description1" style="width: 50%; height: 250px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/63c5d350-3e79-4e40-a964-076f0a8fc470" alt="Description2" style="width: 30%; height: 250px;">
</div>  
  
- Joint Mode, +190 degree (it moves -170 degree)
<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/3a3262ef-8d10-4e57-bcce-2e9fe36ef741" alt="Description1" style="width: 35%; height: 250px; margin-right: 2%;">
  <img src="https://github.com/user-attachments/assets/a7c600c5-4caa-47a2-a05b-8702a5260adc" alt="Description2" style="width: 20%; height: 250px;">
</div>  
<hr style="border-top: 3px solid #bbb;">



## Gain Tuning

During the project, individual tuning was performed using MATLAB simulation to achieve effective control by considering the physical characteristics of the motor and the properties of the control system.  
The values obtained from MATLAB were applied to the actual DC motor for control, and the control was successfully achieved.  
Additionally, the PID gains can be further fine-tuned based on the desired characteristics.





function CommandLego( RobotName, motor1Speed, motor2Speed )
%COMMANDLEGO Summary of this function goes here
%   Detailed explanation goes here
  [IP, brickID] = getConnectionInfo(RobotName);
  currentRobot = legoev3('WiFi',IP,brickID);
mymotor1 = motor(currentRobot, 'B');              % Set up motor
mymotor2 = motor(currentRobot, 'C');              
                               % Motor speed

mymotor1.Speed = motor1Speed;                     % Set motor speed
mymotor2.Speed = motor2Speed;    
start(mymotor1);                            % Start motor
start(mymotor2);
 
clear currentRobot;
pause(0.01);

end


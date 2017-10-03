function StartMotors( currentRobot )
%STARTMOTORS Summary of this function goes here
%   Detailed explanation goes here
mymotor1 = motor(currentRobot, 'B');              % Set up motor
mymotor2 = motor(currentRobot, 'C');              

start(mymotor1);                            % Start motor
start(mymotor2);
end


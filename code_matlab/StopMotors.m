function StopMotors( currentRobot )
%STARTMOTORS Summary of this function goes here
%   Detailed explanation goes here
mymotor1 = motor(currentRobot, 'B');              % Set up motor
mymotor2 = motor(currentRobot, 'C');              

stop(mymotor1);                            % Start motor
stop(mymotor2);
end


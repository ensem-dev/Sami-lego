function SetVelocityOneLego(robot,LinearVelocity,AngularVelocity)

[ls rs] = ConvertVelocities(LinearVelocity,AngularVelocity);
mymotor1 = motor(robot, 'B');              % Set up motor
mymotor2 = motor(robot, 'C'); 

mymotor1.Speed = ls;                     % Set motor speed
mymotor2.Speed = rs;    
     
end
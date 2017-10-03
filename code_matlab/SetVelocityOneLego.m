function SetVelocityOneLego(robot,LinearVelocity,AngularVelocity)

mymotor1 = motor(robot, 'B');              % Set up motor
mymotor2 = motor(robot, 'C'); 

mymotor1.Speed = LinearVelocity;                     % Set motor speed
mymotor2.Speed = LinearVelocity;    
     
end
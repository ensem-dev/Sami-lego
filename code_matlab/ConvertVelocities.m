function [ left_speed right_speed ] = ConvertVelocities( linear_velocity, angular_velocity )
%CONVERTVELOCITIES Summary of this function goes here
%   Detailed explanation goes here
K_LEGO = 0.20095;
LEGO_AXIS = 146.0 ;% 125.0; %entraxe des roues en mm

left_speed = K_LEGO*(linear_velocity-angular_velocity*(2*LEGO_AXIS/2.0));
right_speed = K_LEGO*(linear_velocity+angular_velocity*(2*LEGO_AXIS/2.0));

end


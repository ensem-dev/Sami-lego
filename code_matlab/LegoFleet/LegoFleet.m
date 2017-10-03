%Robots list :
% objectName / IP / brickID
% Lego_01 / 192.168.1.101 / 0016535239bb
% Lego_02 / 192.168.1.107 / 00165352434a
% Lego_03 / 192.168.1.111 / 0016535306c1

classdef LegoFleet < handle

properties
    NumberOfLegoRobots;
    LegoNameArray;
    Network;
    IP;
end



%-------------------------------Methods------------------------------------
methods
% Constructor--------------------------------------------------------------
function Fleet = LegoFleet(NumberOfRobots)
    Fleet.NumberOfLegoRobots = NumberOfRobots;
    temp = strings([NumberOfRobots,1]);
    for i = 1:Fleet.NumberOfLegoRobots
        temp(i) = sprintf('Lego_0%d', i);
    end
    Fleet.LegoNameArray = temp;
    Fleet.Network = zeros(NumberOfRobots,NumberOfRobots);

end


% Declaration of other methods which will be detailled in other m-files ---
%error=ConnectLegoF(LegoFleet,LegoID,Virtual,InitialPosition,InitialOrientation);
%DisconnectLegoF(LegoFleet,LegoID);
% [X Y] = GetPositionLegoF(LegoFleet);
% Phi = GetOrientationLegoF(LegoFleet);
% SetVelocityLegoF(LegoFleet,LinearVelocity,AngularVelocity);
% 
% DefineLegoNetwork(LegoFleet,varargin);
% 
% DisplayLegoF(LegoFleet,varargin);
% CommandLego(RobotName);

%RDV_Static(LegoFleet,FinalRadius,tau,stop);
%GoTo(LegoFleet,FinalPosition,time,Plot,stop);
%ChangeAngle(LegoFleet,FinalAngle,Vlin);
%FleetFormation(LegoFleet,tau,kindofformation,parameters,stop);
%ChangeAngleAndSpeed(LegoFleet,FinalAngle,FinalVlin,time);
%Flocking(LegoFleet,Vlin,time);
end
%------------------------------End Methods---------------------------------



end
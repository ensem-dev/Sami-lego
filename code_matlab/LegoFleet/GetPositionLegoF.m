%Robots list :
% objectName / IP / brickID
% Lego_01 / 192.168.1.101 / 00:16:53:52:39:bb
% Lego_02 / 192.168.1.107 / 00165352434a
% Lego_03 / 192.168.1.111 / 0016535306c1


function [X Y] = GetPositionLegoF(LegoFleet)
X = zeros(LegoFleet.NumberOfLegoRobots,1);
Y = zeros(LegoFleet.NumberOfLegoRobots,1);
for i=1:LegoFleet.NumberOfLegoRobots
    LegoName = char(LegoFleet.LegoNameArray(i));
    P = getPositionCortexByName(LegoName);
    X(i,1) = P(1);
    Y(i,1) = P(2);    
end

end
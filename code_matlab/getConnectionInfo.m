function [IP, brickId] = getConnectionInfo( RobotName )
%Robots list :
% objectName / IP / brickID
% Lego_01 / 192.168.1.100 / 0016535239bb
% Lego_02 / 192.168.1.101 / 00165352434a
% Lego_03 / 192.168.1.102 / 0016535306c1

switch RobotName
    case 'Lego_01'
        IP = '192.168.1.100';
        brickId = '0016535239bb';
    case 'Lego_02'
        IP = '192.168.1.101';
        brickId = '00165352434a';
    case 'Lego_03'
        IP = '192.168.1.102';
        brickId = '0016535306c1';
end

end

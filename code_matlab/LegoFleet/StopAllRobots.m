function StopAllRobots( fleet )
%arrêt de tous les robots
% utile quand on perd la connection wifi
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StopMotors(Current_Lego);
    clear Current_Lego;
end

end


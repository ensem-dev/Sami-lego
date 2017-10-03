fleet = LegoFleet(3);
[x y] = GetPositionLegoF(fleet);
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StarMotors(Current_Lego);
    SetVelocityOneLego(Current_Lego, 10, 10);
   % CommandLego( LegoName, 10, 10 );
    clear Current_Lego;
end


%stoping all motors
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StopMotors(Current_Lego);
    clear Current_Lego;
end

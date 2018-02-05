fleet = LegoFleet(3);
[x y] = GetPositionLegoF(fleet);
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StartMotors(Current_Lego);
    %GOTO (Current_Lego, [X, Y destination], time_to_reach_point, plot,
    %bool stop)
    SetVelocityOneLego(Current_Lego, 0, 0);
   % CommandLego( LegoName, 10, 10 );
    clear Current_Lego;
end

pause(5);
%stoping all motors
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StopMotors(Current_Lego);
    clear Current_Lego;
end

function SetVelocityLegoF(LegoFleet,LinearVelocity,AngularVelocity)
for i=1:LegoFleet.NumberOfLegoRobots
     LegoName = char(LegoFleet.LegoNameArray(i));
     current_robot = ConnectLego(LegoName);
     SetVelocityOneLego(current_robot,LinearVelocity(i,1), AngularVelocity(i,1));
     clear current_robot;
end
end
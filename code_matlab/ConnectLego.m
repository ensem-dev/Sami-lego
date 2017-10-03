function currentRobot = ConnectLego( RobotName )

 [IP, brickID] = getConnectionInfo(RobotName);
  currentRobot = legoev3('WiFi',IP,brickID);

end


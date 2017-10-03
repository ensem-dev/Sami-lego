function Phi = GetOrientationLegoF(LegoFleet)
Phi = zeros(LegoFleet.NumberOfLegoRobots,1);

for i = 1:LegoFleet.NumberOfLegoRobots
    LegoName = char(LegoFleet.LegoNameArray(i));
    orientation = getPositionCortexByName(LegoName);
    Phi(i,1) = orientation(4); %L'orientation dans le plan de la sc�ne est en 4 position
end
%Phi = mod(Phi,2*pi); % D�commenter si orientation voulue en  radians

end
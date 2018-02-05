%N=input('nombre de robots : ');

N=3;
fleet = LegoFleet(N); %déclare une flotte avec 3 robots
for i = 1:fleet.NumberOfLegoRobots
    LegoName = char(fleet.LegoNameArray(i));
    Current_Lego = ConnectLego(LegoName);
    StartMotors(Current_Lego); %démarrage des moteurs du robot
    clear Current_Lego; % clear obligatoire pour switcher d'un robot à l'autre
end

[X Y] = GetPositionLegoF(fleet);
A = zeros(N,N);
thresold = 1000;
for i=1:N
    for j=1:N
        if i~=j && norm([X(i) Y(i)] - [X(j) Y(j)]) < thresold %distance bw i and j
            A(i,j)=1;
        end
    end
end

disp('network');
DefineNetwork(fleet, 'manual',A)


%disp('flocking')
%R.Flocking(80*[1;1.5;1.2;1.2;0.8],1.5);

%R.RDV_Static(10,0.5,0)
FleetFormation(fleet, 2,'circle',500,0)
%FleetFormation(fleet, 2,'line',[1 300],0)
StopAllRobots(fleet);
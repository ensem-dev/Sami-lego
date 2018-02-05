% fleet : la flote de robots
% FinalPosition : tableau de coordonn�es X/Y pour les robots de la flotte
% time : le temps alou� pour rejoindre la cible
% Plot : bool�en pour tracer la trajectoire
% stop : bool�en pour arr�ter (ou pas) les robots en fin de cycle

function Fin=GoTo(fleet,FinalPosition,time,Plot,stop)

if(time == -1)
    test1 = 1;
    test2 =0;
else 
    test1 = 0;
    test2 = 1;
end

Te = 0.1;
Kang =0.2;
Klin = 1;
delta = 146; %entraxe de l'essieu en mm
VangMax = 2*25/delta;

[CPX(:,1) CPY(:,1)] = GetPositionLegoF(fleet);
while (min(abs(CPX(:,1))) > 2000 || min(abs(CPY(:,1))) > 2000) %tant que les valeurs sont ab�rantes on rappelle les
                                                     %coordonn�es
    [CPX(:,1) CPY(:,1)] = GetPositionLegoF(fleet);
end


D = zeros(fleet.NumberOfLegoRobots,1);
for i=1:fleet.NumberOfLegoRobots
   D(i,1) = sqrt((CPX(i,1)-FinalPosition(i,1))^2+(CPY(i,1)-FinalPosition(i,2))^2);
end


if(Plot==1)
        plot(FinalPosition(:,1),FinalPosition(:,2),'ro');
        hold on; %Pour mettre � jour le plot
        plot(CPX(:,1),CPY(:,1),'r+');
end

p=1;
nothere=ones(fleet.NumberOfLegoRobots,1);
t = clock;
while(test1 && any(nothere)|| test2 && (etime(clock,t)<time) && any(nothere))
    
    % Angle
    CurrentOrientation = GetOrientationLegoF(fleet)*pi/180.0;

    RefOrientation = InWhichDirection([CPX(:,p) CPY(:,p)],FinalPosition);
    DiffAngle = RefOrientation - CurrentOrientation;
    
    Vang = Kang*DiffAngle;
    
    %test pour arr�ter les robots quand ils sont arriv�s
    Continue = D(:,p)>delta/4;
    nothere = any(Continue); %si tous les robots sont arriv�s, on sort de la boucle while (nothere = 0)
    %Saturation de la vitesse angulaire.
    ub = Vang>VangMax;
    lb = Vang<-VangMax;
    Vang = Vang.*not(ub | lb)+VangMax.*ub-VangMax.*lb;
    Vang = Vang.*Continue;
    % Mise a jour du critere de test*

    Vlin = Klin*D(:,p);
%     VCmd=min(Vlin,200).*Continue.*1./(1+4*abs(DiffAngle));
    VCmd=min(Vlin,200).*Continue.*1./(1+abs(DiffAngle));

    SetVelocityLegoF(fleet,VCmd,Vang);

    if(Plot==1)
        hold on
        plot(CPX(:,p),CPY(:,p),'b+')
    end

    pause(Te);
    [CPX(:,p+1) CPY(:,p+1)] = GetPositionLegoF(fleet);
    while (min(abs(CPX(:,1))) > 2000 || min(abs(CPY(:,1))) > 2000) %tant que les valeurs sont ab�rantes on rappelle les
                                                     %coordonn�es
        [CPX(:,1) CPY(:,1)] = GetPositionLegoF(fleet);
    end

    for i=1:fleet.NumberOfLegoRobots
          D(i,p+1) = sqrt((CPX(i,p+1)-FinalPosition(i,1))^2+(CPY(i,p+1)-FinalPosition(i,2))^2)
    end

    Fin = [CPX(:,end) CPY(:,end)];
    p=p+1;
end
hold off
%stop all robots
if(stop==1)
    SetVelocityLegoF(fleet, zeros(fleet.NumberOfLegoRobots,1),zeros(fleet.NumberOfLegoRobots,1));
end
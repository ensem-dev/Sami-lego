% fleet : la flote de robots
% FinalPosition : tableau de coordonnées X/Y pour les robots de la flotte
% time : le temps aloué pour rejoindre la cible
% Plot : pour tracer la trajectoire
% stop : booléen pour arrêter (ou pas) les robots en fin de cycle

function Fin=GoTo(fleet,FinalPosition,time,Plot,stop)

if(time == -1)
    test1 = 1;
    test2 =0;
else 
    test1 = 0;
    test2 = 1;
end

Te = 0.01;
%1.1
Kang =2;
Klin = 0.45;
delta = 89;
% VangMax = 2*abs(V)/delta;
VangMax = 2*250/delta;


[CPX(:,1) CPY(:,1)] = GetPositionLegoF(fleet);
D(:,1) = distance([CPX(:,1) CPY(:,1)],FinalPosition);

if(Plot==1)
        plot(FinalPosition(:,1),FinalPosition(:,2),'ro');
        hold on;
        plot(CPX(:,1),CPY(:,1),'r+');
end

p=1;
nothere=ones(fleet.NumberOfLegoRobots,1);
t = clock;
while(test1 && any(nothere)|| test2 && (etime(clock,t)<time) && any(nothere))
    
    % Angle
    CurrentOrientation = GetOrientationLegoF(fleet);
    RefOrientation = InWhichDirection([CPX(:,p) CPY(:,p)],FinalPosition);
    DiffAngle = RefOrientation - CurrentOrientation;
    % Sens
    e1 = DiffAngle>-2*pi & DiffAngle<=-pi;
    e2 = DiffAngle>= 0 & DiffAngle<= pi;
    e3 = DiffAngle>-pi & DiffAngle<0;
    e4 = DiffAngle>pi & DiffAngle<2*pi;
    % e1 = abs(CurrentOrientation)>abs(RefOrientation);

    % Sens = 1*not(e1)-1*e1;
    Sens = 1*(e1+e2) -1*(e3+e4);
    DiffAngle = (-(abs(DiffAngle)-pi).^2+10)/10;%.*t;
    Vang = Kang.*Sens.*abs(DiffAngle);
    %Saturation de la vitesse angulaire.
    ub = Vang>VangMax;
    lb = Vang<-VangMax;
    Vang = Vang.*not(ub | lb)+VangMax.*ub-VangMax.*lb;

    % Mise a jour du critere de test*
    Continue = D(:,p)>10;
    nothere = any(Continue);
    Vlin = Klin*D(:,p);
    VCmd=min(Vlin,200).*Continue.*1./(1+4*abs(DiffAngle));

    SetVelocityLegoF(fleet,VCmd,Vang);

    if(Plot==1)
        hold on
        plot(CPX(:,p),CPY(:,p),'b+')
    end

    pause(Te);
    [CPX(:,p+1) CPY(:,p+1)] = GetPositionLegoF(fleet);
    D(:,p+1) = distance([CPX(:,p+1) CPY(:,p+1)],FinalPosition);

    % [CPX(:,p) FinalPosition(:,1) CPY(:,p) FinalPosition(:,2) D(:,p) Continue Vang VCmd]

    Fin = [CPX(:,end) CPY(:,end)];
    p=p+1;
end
hold off
%stop all robots
if(stop==1)
SetVelocityF(fleet, zeros(LegoFleet.NumberOfLegoRobots,1),zeros(LegoFleet.NumberOfLegoRobots,1));
end
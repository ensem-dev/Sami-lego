function ChangeAngleAndSpeed(KheperaFleet,FinalAngle,FinalVlin,time)

%% Paramètres
Te = 0.01;
Kang=1.1;
epsiA = 0.1*2*pi/360;
VangMax = 2*250/89;


CurrentOrientation = KheperaFleet.GetOrientationF; %reactualisation des positions reelles

test = IsInTheWrongDirection(CurrentOrientation,FinalAngle,epsiA); %Initialisation du critère d'arret


%% Boucle de régulation
t = clock;
while((etime(clock,t)<time) && any(test))
   
% Angle
CurrentOrientation = KheperaFleet.GetOrientationF;   
DiffAngle = FinalAngle - CurrentOrientation;

% Sens
e1 = DiffAngle>-2*pi & DiffAngle<=-pi;
e2 = DiffAngle>= 0 & DiffAngle<= pi;
e3 = DiffAngle>-pi & DiffAngle<0;
e4 = DiffAngle>pi & DiffAngle<2*pi;

Sens = 1*(e1+e2) -1*(e3+e4);
Continue = abs(DiffAngle)>epsiA;
Vang = Kang*Sens.*abs(DiffAngle).*Continue;


% Saturation de la vitesse angulaire.
ub = Vang>VangMax;
lb = Vang<-VangMax;

Vang = Vang.*not(ub | lb)+VangMax.*ub-VangMax.*lb;

% Commande des robots
KheperaFleet.SetVelocityF(FinalVlin,Vang);

pause(Te);

% Reactualisation des Angles
CurrentOrientation = KheperaFleet.GetOrientationF;

test = IsInTheWrongDirection(CurrentOrientation,FinalAngle,epsiA); %reactualisation du critère d'arret
end



 KheperaFleet.SetVelocityF(FinalVlin,zeros(KheperaFleet.NumberOfKhepera,1));


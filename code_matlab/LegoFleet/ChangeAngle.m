function ChangeAngle(KheperaFleet,FinalAngle,Vlin)

Te = 0.01;
Kang= 5;
CurrentOrientation = KheperaFleet.GetOrientationF;
epsiA = 5*2*pi/360;
VangMax = 2*250/89;
test = IsInTheWrongDirection(CurrentOrientation,FinalAngle,epsiA);
while(any(test))
    % Angle
CurrentOrientation = KheperaFleet.GetOrientationF;   
DiffAngle = FinalAngle - CurrentOrientation;
% Sens
e1 = DiffAngle>-2*pi & DiffAngle<=-pi;
e2 = DiffAngle>= 0 & DiffAngle<= pi;
e3 = DiffAngle>-pi & DiffAngle<0;
e4 = DiffAngle>pi & DiffAngle<2*pi;

Sens = 1*(e1+e2) -1*(e3+e4);
Continue = abs(DiffAngle)>5*2*pi/360;
Vang = Kang*Sens.*abs(DiffAngle).*Continue;

%Saturation de la vitesse angulaire.
ub = Vang>VangMax;
lb = Vang<-VangMax;
Vang = Vang.*not(ub | lb)+VangMax.*ub-VangMax.*lb;
KheperaFleet.SetVelocityF(Vlin,Vang);
pause(Te);
CurrentOrientation = KheperaFleet.GetOrientationF;
test = IsInTheWrongDirection(CurrentOrientation,FinalAngle,epsiA);
end
KheperaFleet.SetVelocityF(zeros(KheperaFleet.NumberOfKhepera,1),zeros(KheperaFleet.NumberOfKhepera,1));
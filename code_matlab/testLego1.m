%openCortex();
idObjetLego1 = 1;
idObjetLego2 = 0;
idObjetLego3 = 2;

position1 = getPositionCortex(idObjetLego1);
fprintf('position Lego 1 : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
    position1(1),position1(2),position1(3),position1(4),position1(5));

position2 = getPositionCortex(idObjetLego2);
fprintf('position Lego 2 : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
    position2(1),position2(2),position2(3),position2(4),position2(5));
position3 = getPositionCortex(idObjetLego3);
fprintf('position Lego 3 : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
    position3(1),position3(2),position3(3),position3(4),position3(5));




if (not(exist('lego_01', 'var')))
   % lego_01 = legoev3('WiFi','192.168.1.101','0016535239bb');
  %  lego_02 = legoev3('WiFi','192.168.1.107','00165352434a');
    lego_03 = legoev3('WiFi','192.168.1.111','0016535306c1');
end
%myev3 = legoev3('Bluetooth','COM3');

%mylego = legoev3;
mymotor1 = motor(lego_01, 'B');              % Set up motor
mymotor2 = motor(lego_01, 'C');              
SPEED = 20;                                 % Motor speed

mymotor1.Speed = SPEED;                     % Set motor speed
mymotor2.Speed = SPEED;    
start(mymotor1);                            % Start motor
start(mymotor2);

distance = 0.0;
while(distance < 200.0 )
    postionCourante = getPositionCortex(idObjetLego);
   % fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
   %     postionCourante(1),postionCourante(2),postionCourante(3),postionCourante(4),postionCourante(5));
    distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
   
    %pause(0.01);
end
mymotor1.Speed = 0;                     % Set motor speed
mymotor2.Speed = 0;    
stop(mymotor1);                            % Stop motor
stop(mymotor2);
postionCourante = getPositionCortex(idObjetLego);
distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
fprintf('distance=%f\n',distance);
%exitCortex();
%clearvars;

idObjetLego = 2;

position1 = getPositionCortex(idObjetLego);
fprintf('position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
    position1(1),position1(2),position1(3),position1(4),position1(5));

if (not(exist('lego_03', 'var')))
    lego_03 = legoev3('WiFi','192.168.1.111','0016535306c1');
    %lego3= legoev3('Bluetooth','COM6');
end

%mylego = legoev3;
mymotor1 = motor(lego_03, 'B');              % Set up motor
mymotor2 = motor(lego_03, 'C');              
SPEED = 10;                                 % Motor speed

mymotor1.Speed = SPEED;                     % Set motor speed
mymotor2.Speed = SPEED;    
start(mymotor1);                            % Start motor
start(mymotor2);

distance = 0;
while(distance < 200 )
    postionCourante = getPositionCortex(idObjetLego);
   % fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
   %     postionCourante(1),postionCourante(2),postionCourante(3),postionCourante(4),postionCourante(5));
    distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
    fprintf('distance=%f\n',distance);
    %pause(0.1);
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

idObjetLego = 0;

position1 = getPositionCortex(idObjetLego);
fprintf("position : x=%f, y=%f, z=%f, azimut=%f, ÚlÚvation=%f\n",...
    position1(1),position1(2),position1(3),position1(4),position1(5));

if (not(exist('lego_02', 'var')))
    lego_02 = legoev3('WiFi','192.168.1.107','00165352434a');
    %lego_02= legoev3('Bluetooth','COM3');
end

%mylego = legoev3;
mymotor1 = motor(lego_02, 'B');              % Set up motor
mymotor2 = motor(lego_02, 'C');              
SPEED = 20;                                 % Motor speed

mymotor1.Speed = SPEED;                     % Set motor speed
mymotor2.Speed = SPEED;    
start(mymotor1);                            % Start motor
start(mymotor2);

distance = 0;
while(distance < 200 )
    postionCourante = getPositionCortex(idObjetLego);
   % fprintf("position : x=%f, y=%f, z=%f, azimut=%f, ÚlÚvation=%f\n",...
   %     postionCourante(1),postionCourante(2),postionCourante(3),postionCourante(4),postionCourante(5));
    distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
    %distance=distance+0.01; 
   % pause(0.1);
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

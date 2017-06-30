openCortex();
idObjetLego = 0;

position1 = getPositionCortex(idObjetLego);
fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
    position1(1),position1(2),position1(3),position1(4),position1(5));


%myev3 = legoev3('WiFi','192.168.1.2','00165340e49b');
%myev3 = legoev3('Bluetooth','COM3');

mylego = legoev3;
mymotor1 = motor(mylego, 'B');              % Set up motor
mymotor2 = motor(mylego, 'C');              
SPEED = 20;                                 % Motor speed

mymotor1.Speed = SPEED;                     % Set motor speed
mymotor2.Speed = SPEED;    
start(mymotor1);                            % Start motor
start(mymotor2);

distance = 0;
while(distance < 200 )
    postionCourante = getPositionCortex(idObjetLego);
    fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
        postionCourante(1),postionCourante(2),postionCourante(3),postionCourante(4),postionCourante(5));
    distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
     fprintf('distance=%f\n',distance);
    %wait(0.2);
end

stop(mymotor1);                            % Start motor
stop(mymotor2);
%delete myLego;

%exitCortex();
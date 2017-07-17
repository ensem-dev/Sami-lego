%openCortex();
idObjetLego3 = 1;

if (not(exist('lego_03', 'var')))
    lego_03 = legoev3('WiFi','192.168.1.102','0016535306c1');
end

mymotor1 = motor(lego_03, 'B');              % Set up motor
mymotor2 = motor(lego_03, 'C');              
SPEED = 20;                                 % Motor speed

mymotor1.Speed = SPEED;                     % Set motor speed
mymotor2.Speed = SPEED;    
start(mymotor1);                            % Start motor
start(mymotor2);
for i = 1:4
    position3 = getPositionCortexByID(idObjetLego3);
   
    fprintf('position Lego 3 : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
        position3(1),position3(2),position3(3),position3(4),position3(5));
    mymotor1.Speed = SPEED;                     % Set motor speed
    mymotor2.Speed = SPEED;    
    distance = 0.0;
    oldPosition=position3;
    while(distance < 250.0 )
        positionCourante = getPositionCortexByID(idObjetLego3);
        if(0 == positionCourante(6)) %la position est retournée sans erreur
           oldPosition = positionCourante;
           %fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
           %    postionCourante(1),postionCourante(2),postionCourante(3),postionCourante(4),postionCourante(5));
          distance = sqrt((oldPosition(1)-position3(1))^2+(oldPosition(2)-position3(2))^2);
          fprintf("distance %f\n",distance);
        else
            distance = sqrt((oldPosition(1)-position3(1))^2+(oldPosition(2)-position3(2))^2);
             fprintf("distance (erreur) %f\n",distance);
        end
    end
    %rotation
    azimutFinal = positionCourante(4)+90;
    if(azimutFinal > 180)
        azimutFinal = azimutFinal-360;
    elseif(azimutFinal<-180)
        azimutFinal = azimutFinal+360;
    end
    
    mymotor1.Speed = -10;                     % Set motor speed
    mymotor2.Speed = +10;
    while(abs(positionCourante(4) - azimutFinal )> 5 )
        
        positionCourante = getPositionCortexByID(idObjetLego3);
        
        if(0 == positionCourante(6)) %la position est retournée sans erreur
            if(positionCourante(4) > 180)
                positionCourante(4) = positionCourante(4)-360;
            elseif(positionCourante(4)<-180)
                positionCourante(4) = positionCourante(4)+360;
            end
            oldAngle = positionCourante(4);
            fprintf('position Lego 3 : azimut=%f vers %f\n',positionCourante(4),azimutFinal);
        else
            fprintf('ici\n');
            positionCourante(4) = oldAngle;
        end

    end
end

mymotor1.Speed = 0;                     % Set motor speed
mymotor2.Speed = 0;    

stop(mymotor1);                            % Stop motor
stop(mymotor2);
%postionCourante = getPositionCortex(idObjetLego);
%distance = sqrt((postionCourante(1)-position1(1))^2+(postionCourante(2)-position1(2))^2);
%fprintf('distance=%f\n',distance);
%exitCortex();
%clearvars;

loadlibrary('matlab_cortex.dll', 'matlabclib.h')
x = libpointer('doublePtr',0);
y = libpointer('doublePtr',0);
z = libpointer('doublePtr', 0);
azimut = libpointer('doublePtr', 0);
elevation = libpointer('doublePtr', 0);

% appel des coordonnées de l'objet 1
id_robot = 1;
ret = calllib('matlab_cortex','Get_Position_Cortex', id_robot,x,y,z,azimut,elevation);
fprintf("retour=%i\n", ret)
fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
    get(x,'value'),get(y,'value'),get(z,'value'),get(azimut,'value'),get(elevation,'value'))

% appel des coordonnées de l'objet 2
id_robot = 2;
ret = calllib('matlab_cortex','Get_Position_Cortex', id_robot,x,y,z,azimut,elevation);
fprintf("retour=%i\n", ret)
fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
    get(x,'value'),get(y,'value'),get(z,'value'),get(azimut,'value'),get(elevation,'value'))

% appel des coordonnées de l'objet 3 - il n'existe pas et devrait donné une erreur 
id_robot = 3;
ret = calllib('matlab_cortex','Get_Position_Cortex', id_robot,x,y,z,azimut,elevation);
fprintf("retour=%i\n", ret)
if(ret==1)
    fprintf("il y a une erreur de lecure de la position de l'objet n°%i\n",id_robot)
end
fprintf("position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n",...
    get(x,'value'),get(y,'value'),get(z,'value'),get(azimut,'value'),get(elevation,'value'))

unloadlibrary('matlab_cortex')

% function used by continuous acquisition only !
% lock is acquired by RunAcquisition

function pos = GetPosition(k3)
    pos = zeros(2,1); 
    left = libpointer('int32Ptr',0);
    right = libpointer('int32Ptr',0);
    %tic
    error = calllib('khepera3clib', 'GetEncoderPosition',k3.id,left,right);        
    if error
        ME = MException('GetPosition:CommunicationError', ...
             'The connection with the robot has failed. Check the network and try to reconnect');
        throw(ME);
    end
    %toc
    pos(1) = get(left,'value');
    pos(2) = get(right,'value');
    pos = khepera3.K3toRW(2,pos);
end

function err = openCortex()
    loadlibrary('matlab_cortex.dll', 'matlabclib.h')
    ipCortexServerr = libpointer('string',  "192.168.1.109");
    errorMessage = libpointer('doublePtr');
    err = calllib('Cortex_Matlab', 'getCortexConnexion', ipCortexServerr, errorMessage);
    fprintf("errormessage=%s\n",  get(errorMessage,'value'))
end

function pos = getPosition(idObject)

       %tic
    error = calllib('Cortex_Matlab', 'getObjectPositionCortex', idObject, double* X, double* Y, double* Z, double* azimut, double* elevation);        
    if error
        ME = MException('GetPosition:CommunicationError', ...
             'The connection with the robot has failed. Check the network and try to reconnect');
        throw(ME);
    end
    %toc
    pos(1) = get(left,'value');
    pos(2) = get(right,'value');
    pos = khepera3.K3toRW(2,pos);
 
end

    ----------------------------------------------------
	Your MATLAB license will expire in 38 days.
	Please contact your system administrator or
	MathWorks to renew this license.
    ----------------------------------------------------
>> mylego = legoev3

mylego = 

  legoev3 with properties:

      FirmwareVersion: 'V1.09H'
           HardwareID: '0016535239bb'
            IPAddress: '192.168.1.107'
    CommunicationType: 'WiFi'
         BatteryLevel: 100
     ConnectedSensors: {''  'sonic'  'color'  'gyro'}

>> openCortex

ans =

     1

>> getPositionCortex(2)
Error: File: getPositionCortex.m Line: 1 Column: 32
Unexpected MATLAB expression.
 
>> getPositionCortex(2)
Type was not found.

Error in libpointer (line 17)
 ptr=lib.pointer(varargin{:});

Error in getPositionCortex (line 2)
  x = libpointer('floatPtr',0);
 
>> getPositionCortex(2)
retour=0
position : x=6.444408, y=-500.768494, z=1.279143, azimut=3.028996, élévation=0.000000
>> mymotor1 = motor(mylego, 'B');              % Set up motor
mymotor2 = motor(mylego, 'C');              
>> SPEED = 20;                                 % Motor speed
>> mymotor1.Speed = SPEED;                     % Set motor speed
>> mymotor1.Speed = SPEED;                     % Set motor speed
>> mymotor2.Speed = SPEED;                     % Set motor speed
>> start(mymotor1);                            % Start motor
>> start(mymotor1);                            % Start motor
>> start(mymotor2);                            % Start motor
>> stop(mymotor2);                            % Start motor
>> stop(mymotor1);                            % Start motor
>> getPositionCortex(2)
retour=0
position : x=497.966858, y=104.501152, z=-0.042902, azimut=-76.823441, élévation=0.000000
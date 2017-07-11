function position = getPositionCortexByName(objectName)
  x = libpointer('singlePtr',0);
  y = libpointer('singlePtr',0);
  z = libpointer('singlePtr', 0);
  azimut = libpointer('doublePtr', 0);
  elevation = libpointer('doublePtr', 0);

  ret = calllib('Cortex_Matlab','getObjectPositionCortexByName', objectName, x, y, z, azimut, elevation);
  %fprintf('retour=%i\n', ret)
  %fprintf('position : x=%f, y=%f, z=%f, azimut=%f, élévation=%f\n',...
  %  get(x,'value'),get(y,'value'),get(z,'value'),get(azimut,'value'),get(elevation,'value'))

  position = zeros(6,1);
  position(1) = get(x,'value');
  position(2) = get(y,'value');
  position(3) = get(z,'value');
  position(4) = get(azimut,'value');
  position(5) = get(elevation,'value');
  position(6) = ret;
  
end
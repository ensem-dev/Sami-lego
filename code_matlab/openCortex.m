function err = openCortex()
    loadlibrary('Cortex_Matlab.dll', 'Cortex_Matlab.h')
    ipCortexServer = libpointer('string',  '192.168.1.109');

    %str = repmat(' ',100,1);
    errorMessage = libpointer('cstring', '');

    err = calllib('Cortex_Matlab', 'getCortexConnexion', ipCortexServer, errorMessage);
    %fprintf('errormessage=%s\n',  get(errorMessage,'value'));
    %fprintf('errormessage=%s\n',  errorMessage.value);

    %clear errorMessage;
    
%    unloadlibrary('Cortex_Matlab');

end

function err = exitCortex()
    err = calllib('Cortex_Matlab', 'exitCortexConnexion');
    unloadlibrary('Cortex_Matlab');
end

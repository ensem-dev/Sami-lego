function err = openCortex()
    loadlibrary('Cortex_Matlab.dll', 'Cortex_Matlab.h')
    ipCortexServerr = libpointer('string',  "192.168.1.109");
    errorMessage = libpointer('string');
    err = calllib('Cortex_Matlab', 'getCortexConnexion', ipCortexServerr, errorMessage);
    fprintf("errormessage=%s\n",  get(errorMessage,'value'));
    
    unloadlibrary('Cortex_Matlab');

end

function err = exitCortex()
    err = calllib('Cortex_Matlab', 'exitCortexConnexion');
    unloadlibrary('Cortex_Matlab');
end
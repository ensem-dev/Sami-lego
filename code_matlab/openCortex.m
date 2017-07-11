function err = openCortex()
    loadlibrary('Cortex_Matlab.dll', 'Cortex_Matlab.h')
    ipCortexServer = libpointer('string',  '192.168.1.109');
    %Les deux solutions fonctionne avec un retour err de type char *
    %errorMessage = libpointer('string');
    %errorMessage = libpointer('cstring');%, '                     ');
    
    %# pass c-string by reference
    errorMessage = libpointer('stringPtrPtr',{''}); %# we use a cellarray of strings
    %get(errorMessage)
    
    err = calllib('Cortex_Matlab', 'getCortexConnexion', ipCortexServer, errorMessage);
    fprintf('errormessage=%s\n',  errorMessage.Value{1});
    %clear errorMessage;
    unloadlibrary('Cortex_Matlab');
end


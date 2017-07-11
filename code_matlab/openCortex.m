function err = openCortex()
    loadlibrary('Cortex_Matlab.dll', 'Cortex_Matlab.h')
    assert( libisloaded('Cortex_Matlab') )
    %libfunctions Cortex_Matlab -full
    
    ipCortexServer = libpointer('string',  '192.168.1.109');
    
    %# pass c-string by reference
    errorMessage = libpointer('stringPtrPtr',{''}); %# we use a cellarray of strings
    %get(errorMessage)

    err = calllib('Cortex_Matlab', 'getCortexConnexion', ipCortexServer, errorMessage);
    if(0 == err)
        fprintf('error message = %s\n',  errorMessage.Value{1});
    else
        fprintf('%s\n',  errorMessage.Value{1});
    end
    clear errorMessage;
    
    %unloadlibrary('Cortex_Matlab');
end


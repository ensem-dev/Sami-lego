function err = exitCortex()
    err = calllib('Cortex_Matlab', 'exitCortexConnexion');
    unloadlibrary('Cortex_Matlab');
end
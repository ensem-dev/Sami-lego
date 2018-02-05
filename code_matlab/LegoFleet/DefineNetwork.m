function DefineNetwork(Fleet,varargin)
optargin = size(varargin,2);
if(optargin>2)
error('Too many options in method DefineNetwork of KheperaFleet (max 2)');
elseif(optargin==0)
error('Please make a choice');
end



    if(strcmp(varargin{1},'manual'))
       Fleet.Network = varargin{2};
    elseif(strcmp(varargin(1),'random'))
        disp('Still not implemented!');
    elseif(strcmp(varargin{1},'proximity'))
        threshold = varargin{2};
        [X Y] = GetPositionLegoF(Fleet);
        for i=1:Fleet.NumberOfLegoRobots-1
            for j=i+1:Fleet.NumberOfLegoRobots
                if(distance([X(i,1) Y(i,1)],[X(j,1) Y(j,1)])<=threshold)
                    Fleet.Network(i,j)=1;
                    Fleet.Network(j,i)=1;
                else
                    Fleet.Network(i,j)=0;
                    Fleet.Network(j,i)=0;
                end
            end
        end
    else
        error('This is not a valid option for DefineNetwork option');
    end
        

end
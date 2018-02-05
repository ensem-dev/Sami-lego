function FleetFormation(Fleet,tau,kindofformation,parameters,stop)

% kind of formation : 'circle' -> parameters = radius
% 'line' -> parameters = [pente support]
% 'V' -> parameters = [pente support]
% 'other' -> parameters = [fx fy] formation

n = Fleet.NumberOfLegoRobots;
epsilon = 5;
A = Fleet.Network;
L = diag(sum(A(:,:)))-A;
epsi = 1/(max(max(L))+1);
Pe = eye(n)-epsi*L;




if(strcmp(kindofformation,'circle'))
    F = parameters*exp(1i*[0:2*pi/n:2*pi-2*pi/n]);
    fx = real(F)';
    fy = imag(F)';
elseif(strcmp(kindofformation,'line'))
    length  = parameters(2);
    k = parameters(1);
    fx = [0:length/n:length-length/n]';
    fy = k*fx;
elseif(strcmp(kindofformation,'v'))
    k = parameters(1);
    length = parameters(2);
    fx = [0:length/n:length-length/n]';
    fy(1:ceil(n/2)) = k*fx(1:ceil(n/2));
    fy(ceil(n/2)+1:n) = -k*fx(ceil(n/2)+1:n)+fy(ceil(n/2))+k*fx(ceil(n/2));
    fy = fy';
else
    fx = parameters(:,1);
    fy = parameters(:,2);
end



% Formation consensus

[CPX(:,1) CPY(:,1)] = GetPositionLegoF(Fleet);
tX(:,1) = CPX(:,1);
tY(:,1) = CPY(:,1);

d = 10^9;
indice = zeros(n,1);
for i=1:n
    d = 10^9;
    for j=1:n
    t = distance([CPX(i,1) CPY(i,1)],[fx(j) fy(j)]);
        if(t<d & j~=indice)
            d = t;
            indice(i)=j;
        end
    end
end
tempx = fx;
tempy = fy;

% min([CPX(:,1) CPY(:,2)],[fx(indice) fy(indice)]);

for i = 1:n
fx(i) = tempx(indice(i));
fy(i) = tempy(indice(i));
end

FormationDone = 0*ones(n,1);
i = 1;
    figure(2)

while(any(not(FormationDone)))
    pause(0.5)
%     disp('boucle while')
%     tX(:,i+1) = Pe*tX(:,i)+epsi*L*fx; 
%     tY(:,i+1) = Pe*tY(:,i)+epsi*L*fy;
%     d2 = distance([tX(:,i+1) tY(:,i+1)],[tX(:,i) tY(:,i)]);
%     u=0;
% while(any(d2<10) && u<100)
%     tX(:,i+1) = Pe*tX(:,i)+epsi*L*fx; 
%     tY(:,i+1) = Pe*tY(:,i)+epsi*L*fy;
%     d2 = distance([tX(:,i+1) tY(:,i+1)],[tX(:,i) tY(:,i)]);
%     u=u+1;
% end
    tX = Pe*CPX(:,i)+epsi*L*fx; 
    tY = Pe*CPY(:,i)+epsi*L*fy;
    
%     tempx = tX;
%     tempy = tY;
%     u=0;
%     d2 = distance([tX tY],[CPX(:,i) CPY(:,i)]);
%     test = d2<30;
% while(any(test) && u<100)
%     tX = (Pe*tempx+epsi*L*fx).*test + tempx.*not(test); 
%     tY = (Pe*tempx+epsi*L*fy).*test + tempy.*not(test);
%     d2 = distance([tX tY],[CPX(:,i) CPY(:,i)]);
%     test = d2<30;
%     tempx = tX;
%     tempy = tY;
%     u=u+1;
% 
% end

    GoTo(Fleet, [tX,tY],tau,1,0);
    i = i+1;
    [CPX(:,i) CPY(:,i)] = GetPositionLegoF(Fleet);
%     d = distance([tX(:,i) tY(:,i)],[tX(:,i-1) tY(:,i-1)]);
%     FormationDone = d<epsilon;
    %d = distance([CPX(:,i) CPY(:,i)],[CPX(:,i-1) CPY(:,i-1)]);
    d = distance([CPX(:,i) CPY(:,i)],[CPX(:,i-1) CPY(:,i-1)])

    FormationDone = d<50;
%     [CPX(:,i) CPY(:,i) CPX(:,i-1) CPY(:,i-1) d FormationDone]

end


% for i = 2:size(tX,2)
%     %disp('boucle for')
% 
%     KheperaFleet.GoTo([tX(:,i),tY(:,i)],tau,1,stop);
%     [CPX(:,i) CPY(:,i)] = KheperaFleet.GetPositionF;
% end
SetVelocityLegoF(Fleet, zeros(Fleet.NumberOfLegoRobots,1),zeros(Fleet.NumberOfLegoRobots,1));
figure (1)
hold on
for i = 1:size(CPX,2)-1
    plot(CPX(:,i),CPY(:,i),'b.')
    hold on
end
plot(CPX(:,end),CPY(:,end),'r*')
% plot(fx,fy,'g-');
hold off

% for i=1: size(A,2)
%     for j = 1:size(A,2)
%         if(A(i,j) == 1)
%             plot([CPX(i,end) CPY(i,end)],[CPX(j,end) CPY(j,end)],'m')
%         end
%     end
% end
% hold off
end
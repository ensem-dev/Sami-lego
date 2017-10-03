function Phi = InWhichDirection(CurrentPosition,GoalPosition)


X = -CurrentPosition(:,1)+GoalPosition(:,1);
Y = -CurrentPosition(:,2)+GoalPosition(:,2);

Phi(:,1) = atan2(Y,X);

for i=1:size(Phi,1)
    if(Phi(i,1)<0)
        Phi(i,1)= 2*pi+Phi(i,1);
    end
end
end
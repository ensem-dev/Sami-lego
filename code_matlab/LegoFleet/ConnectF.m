function error=ConnectF(KheperaFleet,KheperaID,Virtual,InitialPosition,InitialOrientation)


for i=1:KheperaFleet.NumberOfKhepera
%     Number = KheperaID; %= num2str(KheperaID(i));
    error(i)=KheperaFleet.KheperaArray(i,1).Connect(KheperaID(i),Virtual(i),[InitialPosition(i,1) InitialPosition(i,2)],InitialOrientation(i));
end

end
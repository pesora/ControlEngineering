Kp=60; %input("Proportional Gain(Kp)=");
K=1;
Tconst=0.024;
omegan=sqrt(Kp*K/Tconst)
cheta=1/(2*Tconst*omegan)
num=[omegan^2];
den=[1 2*cheta*omegan omegan^2]; 
printsys(num,den)

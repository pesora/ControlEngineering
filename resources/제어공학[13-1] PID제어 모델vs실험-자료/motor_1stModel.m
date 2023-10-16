R=16.4;L=0.0;
J=3.8e-7;b=0,0;
Ke=3.2086e-2;
Kt=3.21e-2;
Tconst=R*J/(R*b+Ke*Kt);
K=Kt/(R*b+Ke*Kt);
Ka=24/1500;
Kc=2048/2/3.141592;
Kp=1;Ki=0;Kd=0;
K1=1/K;K2=Tconst/K;K0=R/Kt;
num=[K];
den=[Tconst 1];
printsys(num,den)




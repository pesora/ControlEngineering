function [y]=lsim2ndPI(t,r,Kp,Ki)
K=1;Tconst=0.024;
numc=[Kp Ki];
denc =[1 0];
nump=[K];
denp=[Tconst 1 0];
[num,den]=series(numc,denc,nump,denp);
[num,den]=cloop(num,den,-1);
y=lsim(num,den,r,t); %general response for

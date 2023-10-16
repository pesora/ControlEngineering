function [t,r,y]=Ramp2ndKI(Kp,Ki)
K=1;Tconst=0.024;
numc=[Kp Ki];
denc=[1 0];
nump=[K];
denp=[Tconst 1 0];
[num,den]=series(numc,denc,nump,denp);
[num,den]=cloop(num,den,-1);
t=[0:0.001:0.3];
for i=1:1:301,
    r(i)=1;
end
y=lsim(num,den,r,t);

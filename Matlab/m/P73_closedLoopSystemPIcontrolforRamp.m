Kp=50;Ki=100;K=1;Tconst=0.024;
numc=[Kp Ki];
denc=[1 0];
nump=[K];
denp=[Tconst 1 0];
[num,den]=series(numc,denc,nump,denp);
[num,den]=cloop(num,den,-1);
t=[0:0.001:1];
r=t;
y=lsim(num,den,r,t);
ess=r(1001)-y(1001)
plot(t,y,'-b',t,r,'r--')

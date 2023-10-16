Kp=50;K=1;Tconst=0.024;
numc=[Kp];
denc=[1];
nump=[K];
denp=[Tconst 1 0];
[num,den]=series(numc,denc,nump,denp);
[num,den]=cloop(num,den,-1);
t=[0:0.001:0.3];
r=t;
y=lsim(num,den,r,t);
ess=r(301)-y(301)
plot(t,y,'-b',t,r,'r--')

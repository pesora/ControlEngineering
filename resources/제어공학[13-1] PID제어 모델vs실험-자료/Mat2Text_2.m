load('MotorConResult','-mat');
y=ans';
fp=fopen('yout_model.xls','w');
for i=1:1:1001
    if mod(i,10)==1
        fprintf(fp,'%12.8f\t%12.8f\r\n',y(i,1),y(i,2));
        [y(i,1) y(i,2)]
    end
end
fclose(fp);
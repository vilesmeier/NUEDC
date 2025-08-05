s=tf('s');
z = tf('z',1/20000);
syms k;
%hold on;

%% Paramater
% Inductor/Capacitor Calculation
Iout = 1000;        % DC output Current in A
Vdc  = 400;         % DC output voltage in V
Vac  = 30;          % AC input voltage in V
f_ac = 50;           % AC input frequent in Hz
THD  = 0.1;          % AC input current THD in %

% paramater for rectifier
L=0.004;     % AC inductor
RL= 0.005;   % DC resistor of inductor
C=4400*1e-6;  % DC Capacitor
Rdc=100;      % DC Load

% digital control Loop paramater
kpu=4;
kiu=15;
kpi=0.50;
kii=500;
w0=100*pi;
wc=2*pi;

%% DC capacitor calculation
% output magnitue is the amplitude of ripple
filter = Iout*Rdc/(Rdc*C*s+1);
[output_meg,output_phase] = bode(filter,200*pi);
display(output_meg);

%% AC inductor calculation
% maximum
L_max = (Vdc-Vac)/(2*pi*f_ac*Iout);
display(L_max);
% minimum
% simplify:ripple 0.2% ,calculate ripple in 2/5/7/9/11/13
L_min = double((0.002*Vac*sqrt(symsum(1/((2*k+1)^2),k,1,6)))/(Iout*THD*f_ac*2*pi));
display(L_min);



%% Transfer function analysis for current loop
% QPR transfer function
Gqpr  = (kpi + (2*kii*wc*s)/(s^2+2*wc*s+w0*w0));
% open loop transfer function
Gi_op = Gqpr/(L*s+RL);
% close loop transfer function
Gi_cl = Gi_op/(1+Gi_op);

% current loop analysis
poles_i = pole(Gi_cl);
display(poles_i);

%% transfer function analysis for voltage loop
% PID transfer function
Gpid = kpu*kiu/s;
% output filter transfer function
Go=Rdc/(1+C*Rdc*s);
%open loop transfer function
Gu_op=Gpid*Gi_cl*Go;
%close loop transfer function
Gu_cl=Gu_op/(1+Gu_op);

%current loop analysis
%bode(Gu_op);
%bode(Gu_cl);
poles_u = pole(Gu_cl);
display(poles_u);

Ts = 20000;

%% discrete transfer function
Dis_Gqpr = c2d(Gqpr,1/20000,'tustin');
Dis_Gpid = c2d(Gpid,1/20000,'tustin');

Gtf_50=(s^2+100*100*pi*pi)/(s^2+50*s+100*100*pi*pi);
Gdtf_50 = c2d(Gtf_50,1/Ts,'tustin');

Gtf_100=(s^2+100*100*pi*pi*2*2)/(s^2+50*s+100*100*pi*pi*2*2);
Gdtf_100 = c2d(Gtf_100,1/Ts,'tustin');

Gtf_150=(s^2+100*100*pi*pi*3*3)/(s^2+50*s+100*100*pi*pi*3*3);
Gdtf_150 = c2d(Gtf_150,1/Ts,'tustin');


hold on;
bode(Gdtf_100);

Gdtest = tf([4.0009861,-7.99802828,4.0009861],[4.00598621,-7.99802828,3.9959898],1/20000);


bode(Gdtest);
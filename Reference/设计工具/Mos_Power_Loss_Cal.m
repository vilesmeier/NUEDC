%% MOSFET Power Loss Calculator
% Date:2025/07
% Author:
% This Calculator is used for calculate MOSFET power loss in power
% electronic converters

%% MOSFET Parameters 
% Power Converter Parameters
% Vds
Vdsrms = 60;
% Id
Idrms = 3;
% Switching Frequency
fsw = 20000;

% MOSFET Parameters
% On Resistence
Rdson = 7.3*1e-3;
% Output Capacitor
Coss = 320*1e-12;
% Internal Series Rg
Rg_mos = 1.1;
% External series Rg
Rg = 1.2;
% Total Gate Charge
Qg = 28*1e-9;
% Charge for Cgd
Qgd = 6.1*1e-9;
% Charge for Cgs
Qgs= 9.8*1e-9;
% Gate Charge @Vgs = Vgsth
Qgth = 6*1e-9;

% Boot Diode Parameters
Vforward = 0.7;

% Gate Driver IC Parameters
% Output Resister @ OUT = HIGH
Roh = 5;
% Output Resister @ OUT = LOW
Rol = 0.55;
%Gate Drive Voltage
Vdrive = 12;

% Calculated Vaules
Qgs1 = Qgth;
Qgs2 = Qg-Qgth;
Rg_total = Rg+Rg_mos;
Ig = (Vdrive-Vforward)/(Roh + Rg_total);

%% Power Loss Calculate
% Conduction Losses
Pc = Idrms * Idrms * Rdson;
% Switching Losses
Psw = Vdsrms * Idrms * fsw * (Qgs2+Qgd) / Ig;
% Gate Charge Losses
Pgate = Qg * Vdrive * fsw;
% Gate Drive IC Losses
Pdrive = Vdrive * Qg * fsw * 0.5 * (Roh/(Roh + Rg_total) + Rol/(Rol + Rg_total));
% Outoput Capacitor Losses
Poss = 0.5 * Vdsrms * Vdsrms * fsw * Coss;
% Total Power Loss
Ptotal = Pc + Psw + Pgate + Pdrive + Poss;
display(Ptotal);

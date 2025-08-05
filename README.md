# NUEDC  

2025全国大学生电子设计竞赛备赛资料  
赛题方向：电力电子（电源）  

本仓库内资料全部开源,并**遵循开源协议GPL-V3**  
**使用本仓库内代码造成的后果,本人概不负责**  
开源协议见Main Branch内LICENSE.md  

特别鸣谢：  

***山东大学电气工程学院 电力电子化能源系统实验室（MPEES）***  
***山东大学电工电子创新教育平台***  

## 开发环境

软件部分：  
TMS320F28335  
CCS12.4  
C2000Ware_5_00_00_00  
C2000Ware_DigitalPower_SDK_5_02_00_00  
C2000Ware_MotorControl_SDK_5_02_00_00  

硬件部分：  
嘉立创EDA Pro  

仿真部分：  
Matlab2024a  
PLECS 4.7  
Multisim14  

## /Code  

模块化功能代码  
控制环路：PID、PR、Transform、MPPT、PLL、SVPWM  
信号处理：RMS、FFT、PowerAnalyzer  
片内外设：ADC、PWM、CAP、SPI、IIC、UART、XINT、GPIO  
片外外设：OLED（0.96'）、TFT（1.8'）、ADDA、继电器、键盘  
通用文件：state、global  

## /DataSheet  

所用芯片的数据手册、DSP的数据手册、用户手册  

## /Matlab  

Matlab Simulink数字环路仿真  
Matlab还支持对C2000的MBD（module based development，基于模型的开发），还能搞代码生成，据说能做到一行代码不写速通电赛。**整明白的大佬可以教教我，我反正不会**  

## /Multisim 

Multisim 模拟环路仿真  

## /PCB  

硬件PCB工程  
单相/三相H桥、采样、FSBB、UCC28180-PFC、无控整流、ACAC  

## /Question  

全国大学生设计竞赛往年赛题、电源题汇总、往年赛题分类  

## /Reference  

相关参考资料、部分芯片的设计工具、设计参考手册  

## /Solutions  

往年赛题的完整题解（软件部分），硬件模块见 /PCB 文件夹  

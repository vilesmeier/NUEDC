# NUEDC  
2025全国大学生电子设计竞赛备赛资料  
备赛方向：电力电子  

特别鸣谢：  

***山东大学电气工程学院 电力电子化能源系统实验室（MPEES）***  
***山东大学电工电子创新教育平台***  

- [NUEDC](#nuedc)
  - [License](#license)
  - [开发环境](#开发环境)
    - [软件](#软件)
      - [TMS320F28335](#tms320f28335)
      - [TMS320F28379D/TMS320F280049C](#tms320f28379dtms320f280049c)
    - [硬件](#硬件)
    - [仿真](#仿真)
  - [文件结构](#文件结构)
  - [Later Updates](#later-updates)


## License
本仓库内资料全部开源,并**遵循开源协议GPL-V3**  
**使用本仓库内代码造成的后果,本人概不负责**  
开源协议见Main Branch内LICENSE.md  

## 开发环境
### 软件
#### TMS320F28335
CCS12.4  
C2000Ware_5_00_00_00  
C2000Ware_DigitalPower_SDK_5_02_00_00  
C2000Ware_MotorControl_SDK_5_02_00_00  

#### TMS320F28379D/TMS320F280049C
CCS20.20  
C2000Ware_5_05_00_00  
C2000Ware_DigitalPower_SDK_5_05_00_00  
C2000Ware_MotorControl_SDK_5_04_00_00 

### 硬件  
嘉立创EDA Pro  

### 仿真  
Matlab2024a  
PLECS 4.7  
Multisim14  

## 文件结构
``` 
NUEDC                           根目录
├── Code                        模块化代码
│   ├── Control_Driver          控制环路代码
│   └── Peripheral_Driver       DSP硬件层代码
│       ├── 28335               TMS320F28335硬件驱动
│       ├── 28379D              TMS320F28379D硬件驱动
│       └── 280049C             TMS320F280049C硬件驱动
├── DataSheet                   数据手册
│   ├── TMS320F28335            TMS320F28335相关手册、核心板资料
│   ├── TMS320F28379D           TMS320F28379D相关手册、核心板资料
│   ├── TMS320F280049C          TMS320F280049C相关手册、核心板资料
│   ├── 实验室仪器               实验室仪器说明书
│   └── 芯片                    所用器件的数据手册
├── Matlab                      Matlab-Simulink仿真
├── Multisim                    Multisim模拟回路仿真
├── PCB                         PCB工程
│   ├── Library                 嘉立创自建库
│   ├── unuse_projects          优化前旧工程
│   └── projects                优化后新工程
├── PLECS                       PLECS变流器仿真
├── Projects                    代码工程
│   ├── 28335                   28335主控
│   ├── 28379D                  28379D主控
│   └── 280049C                 280049C主控
├── Question                    电子设计竞赛历年真题及综合测评真题
└── Reference                   参考资料
    ├── Related Articles        相关论文
    ├── 设计工具                TI芯片设计工具
    ├── 设计手册                TI相关设计手册
    └── 综合测评资料            综合测评用参考资料
```
## Later Updates
后续更新将不再使用TMS320F28335作为主控  
主控芯片更换为TMS320F28379D  
旧工程可能会移植至TMS320F28379D平台  


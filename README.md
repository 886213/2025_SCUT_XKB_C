# 2025_SCUT_XKB_C

## 项目概述

本项目作品获2025年华南理工星坤杯电子设计竞赛暨全国大学生电子设计竞赛初赛C题运动目标控制与自动追踪系统二等奖，项目包含两个子系统——运动目标控制系统和自动追踪系统，相关硬件有STM32H750VBT6系统板、OPENMV、OV2640摄像头、42步进电机、步进电机驱动、4.0寸电容触摸屏、2.0寸IPS串口屏、激光笔驱动、3D打印零件等，关键软件算法有色块识别、步进电机驱动算法、匿名助手上位机等，使用的软件有keilv5、OPENMVIDE、SolidWorks、匿名上位机、VSCODE、嘉立创EDA专业版等。

## 文件内容介绍

hardware中是作品所使用的硬件部分，包括所设计的PCB（基于嘉立创EDA专业版工程），购买的模块链接和3D建模装配体零件。
software中是作品的工程源码。

## hardware文件内容详细介绍

### 相关模块文件夹内容介绍

主控系统板STM32H750VBT6 【淘宝】<https://e.tb.cn/h.h9ZpijjhLlQnO9Y?tk=fpGv4dZid4E>

摄像头模块OV2640 【淘宝】<https://e.tb.cn/h.hQnG1QoUQ2CDAn5?tk=EpLg4dZiYWE>

步进电机驱动模块DRV8825 【淘宝】<https://e.tb.cn/h.hQl1n0NjAsIuD6K?tk=qYrt4dZ8TUt>

4.0寸SPI串口 TFT液晶屏电容触摸屏 【淘宝】<https://e.tb.cn/h.hQnuHVjiUumDyGg?tk=3Y5p4dZ86p1>

2.0寸tft lcd显示屏 【淘宝】<https://e.tb.cn/h.h9ZrNzqqyMheQXh?tk=DVco4dZjxYR>

【淘宝】<https://e.tb.cn/h.hQM6A3ldc3r51hT?tk=hNYk4dZCHKw>

【淘宝】<https://e.tb.cn/h.hQMSksNuxpaBm98?tk=wjRF4dZyzYT>

TTL转nRF24L01无线串口【淘宝】<https://e.tb.cn/h.hQlLpgA0ttnL6zy?tk=fhtl4d0aWBWhttps://e.tb.cn/h>.

USB转nRF24L01无线串口模块 <https://e.tb.cn/h.h90UW1Q0fzz2jTr?tk=3Z1g4d0Z3F0>

### jlcProject文件夹内容介绍

内含的jlcProject.epro文件请使用嘉立创EDA专业版导入嘉立创EDA专业版工程，工程共涉及五个PCB板，分别是2步进电机驱动板、电源板、可调电压输出板、人机交互板、主控板。

### SolidWorksProject文件夹内容介绍

内含涉及的模型文件

## software文件内容详细介绍

### projectV3工程介绍

projectV3是运动目标控制系统的工程

### project2V0工程介绍

project2V0是自动追踪系统的工程

## 联系方式

如有问题可联系vx：ljs3038393858
或qq：3038393858

## 版本更新

| 版号 | 日期      | 更新内容   |
| ---- | --------- | ---------- |
| V0.0 | 2025.7.19 | 项目初始化 |

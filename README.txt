HC-SR501红外传感器, 感应角度小于100度，最小约0.5秒延时封锁。测试有效读数在5米内。超过5米读数不够灵敏。
接线方式：电源+ 接 nano5V、输出口 接 nano 9口、电源-（挨芯片最近的口）接 nano GND

RCWL-0516微波雷达传感器，感应角度小于120度，说明书上写最大感应距离9米，实际测试最远感应4米多些，超过4米读数不够灵敏。也有延时封锁，但是读数感应比HC-SR501红外感应更灵敏。
接线方式：3V3 接 nano3.3V、OUT 接 nano 9口、GND 接 nano GND

HC-SR04/US-015超声波雷达传感器，感应角度小于15度，2厘米-4米感应距离。
接线方式：VCC 接 nano5V、Trig 接 nano 10口、Echo 接 nano 9口、GND 接 nano GND

AJ-SR04M超声波雷达传感器，感应角度小于75度，实际测试4米内读数准确，超过4米读数不够灵敏，超过5米基本没有读数。
接线方式：5V 接 nano 5V、Trig 接 nano 10口、Echo 接 nano 9口、GND 接 nano GND

MAX4466声音传感器，需要对着麦克风，才能感知到。
接线方式：5V 接 nano 5V、OUT 接 nano A0口、GND 接 nano GND

三线漫反射式红外光电+蓝牙做计数系统。
蓝牙VCC 接 nano 5V、GND 接 nano GND 、RX 接 nano TX、TX 接 nano RX
光电黑线 红线 接 24V+、蓝线 接 24V-、接 nano 3口、蓝线 接 nano GND，由于光电输出工作电压24V所有中间加分压电路，到nano的电压要到5V以内。
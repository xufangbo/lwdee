~~~bash
 # linux查看物理核
 cat /proc/cpuinfo | grep "physical id" | sort | uniq | wc -l
 # linux查看逻辑核
 cat /proc/cpuinfo| grep "processor"| wc -l

 # 简单命令
 lscpu
~~~
https://blog.csdn.src/imxiangzi/article/details/50482257
https://blog.csdn.src/highhandli/article/details/105229750

这个人遇到的问题跟我一样
https://blog.csdn.src/highhandli/article/details/105229750


https://blog.csdn.src/zjq2008wd/article/details/39371819
https://zhuanlan.zhihu.com/p/361750240

惊群现象
https://zhuanlan.zhihu.com/p/385410196
https://www.cnblogs.com/Anker/p/7076537.html

/usr/local/lib/libprotobuf-lite.a


ulimit -n 查看

https://zhuanlan.zhihu.com/p/538934888
临时修改
ulimit -n 1048576

永久修改
修改vim /etc/security/limits.conf配置文件：
在文件末尾添加：
*    hard    nofile    1048576
*    soft    nofile    1048576


2022-04-05 客户端和服务端的TCP都上不来，服务端的TCP只有300左右，不管在T420还是P50上都是这个现象。虚拟机作为client的TPS能力很弱开始有些数后来基本为0。不清楚是客户端的问题还是服务端的问题。下一步到集群上去测试。
######    k8s-node01     #######
docker stop terasort1
docker stop terasort2
docker stop terasort3
docker stop terasort4
docker stop terasort5
docker stop terasort6
docker stop terasort7
docker stop terasort8
docker stop terasort9
docker stop terasort10
docker stop terasort11
docker stop terasort12
docker stop terasort13
docker stop terasort14
docker stop terasort15
######    k8s-node02     #######
docker stop terasort16
docker stop terasort17
docker stop terasort18
docker stop terasort19
docker stop terasort20
docker stop terasort21
docker stop terasort22
docker stop terasort23
docker stop terasort24
docker stop terasort25
docker stop terasort26
docker stop terasort27
docker stop terasort28
docker stop terasort29
docker stop terasort30
######    k8s-node03     #######
docker stop terasort31
docker stop terasort32
docker stop terasort33
docker stop terasort34
docker stop terasort35
docker stop terasort36
docker stop terasort37
docker stop terasort38
docker stop terasort39
docker stop terasort40
docker stop terasort41
docker stop terasort42
docker stop terasort43
docker stop terasort44
######    k8s-node04     #######
docker stop terasort45
docker stop terasort46
docker stop terasort47
docker stop terasort48
docker stop terasort49
docker stop terasort50
docker stop terasort51
docker stop terasort52
docker stop terasort53
docker stop terasort54
docker stop terasort55
docker stop terasort56
docker stop terasort57
docker stop terasort58
######    k8s-node05     #######
docker stop terasort59
docker stop terasort60
docker stop terasort61
docker stop terasort62
docker stop terasort63
docker stop terasort64
docker stop terasort65
docker stop terasort66
docker stop terasort67
docker stop terasort68
docker stop terasort69
docker stop terasort70
docker stop terasort71
docker stop terasort72

######    k8s-node01     #######
docker rm terasort1 
docker rm terasort2 
docker rm terasort3 
docker rm terasort4 
docker rm terasort5 
docker rm terasort6 
docker rm terasort7 
docker rm terasort8 
docker rm terasort9 
docker rm terasort10 
docker rm terasort11 
docker rm terasort12 
docker rm terasort13 
docker rm terasort14 
docker rm terasort15 
######    k8s-node02     #######
docker rm terasort16 
docker rm terasort17 
docker rm terasort18 
docker rm terasort19 
docker rm terasort20 
docker rm terasort21 
docker rm terasort22 
docker rm terasort23 
docker rm terasort24 
docker rm terasort25 
docker rm terasort26 
docker rm terasort27 
docker rm terasort28 
docker rm terasort29 
docker rm terasort30 
######    k8s-node03     #######
docker rm terasort31 
docker rm terasort32 
docker rm terasort33 
docker rm terasort34 
docker rm terasort35 
docker rm terasort36 
docker rm terasort37 
docker rm terasort38 
docker rm terasort39 
docker rm terasort40 
docker rm terasort41 
docker rm terasort42 
docker rm terasort43 
docker rm terasort44 
######    k8s-node04     #######
docker rm terasort45 
docker rm terasort46 
docker rm terasort47 
docker rm terasort48 
docker rm terasort49 
docker rm terasort50 
docker rm terasort51 
docker rm terasort52 
docker rm terasort53 
docker rm terasort54 
docker rm terasort55 
docker rm terasort56 
docker rm terasort57 
docker rm terasort58 
######    k8s-node05     #######
docker rm terasort59 
docker rm terasort60 
docker rm terasort61 
docker rm terasort62 
docker rm terasort63 
docker rm terasort64 
docker rm terasort65 
docker rm terasort66 
docker rm terasort67 
docker rm terasort68 
docker rm terasort69 
docker rm terasort70 
docker rm terasort71 
docker rm terasort72 

######    k8s-node01     #######
docker start terasort1 
docker start terasort2 
docker start terasort3 
docker start terasort4 
docker start terasort5 
docker start terasort6 
docker start terasort7 
docker start terasort8 
docker start terasort9 
docker start terasort10 
docker start terasort11 
docker start terasort12 
docker start terasort13 
docker start terasort14 
docker start terasort15 
######    k8s-node02     #######
docker start terasort16 
docker start terasort17 
docker start terasort18 
docker start terasort19 
docker start terasort20 
docker start terasort21 
docker start terasort22 
docker start terasort23 
docker start terasort24 
docker start terasort25 
docker start terasort26 
docker start terasort27 
docker start terasort28 
docker start terasort29 
docker start terasort30 
######    k8s-node03     #######
docker start terasort31 
docker start terasort32 
docker start terasort33 
docker start terasort34 
docker start terasort35 
docker start terasort36 
docker start terasort37 
docker start terasort38 
docker start terasort39 
docker start terasort40 
docker start terasort41 
docker start terasort42 
docker start terasort43 
docker start terasort44 
######    k8s-node04     #######
docker start terasort45 
docker start terasort46 
docker start terasort47 
docker start terasort48 
docker start terasort49 
docker start terasort50 
docker start terasort51 
docker start terasort52 
docker start terasort53 
docker start terasort54 
docker start terasort55 
docker start terasort56 
docker start terasort57 
docker start terasort58 
######    k8s-node05     #######
docker start terasort59 
docker start terasort60 
docker start terasort61 
docker start terasort62 
docker start terasort63 
docker start terasort64 
docker start terasort65 
docker start terasort66 
docker start terasort67 
docker start terasort68 
docker start terasort69 
docker start terasort70 
docker start terasort71 
docker start terasort72 


######    k8s-node01     #######
docker run --name terasort1  -e nodename=node1 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort2  -e nodename=node2 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort3  -e nodename=node3 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort4  -e nodename=node4 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort5  -e nodename=node5 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort6  -e nodename=node6 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort7  -e nodename=node7 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort8  -e nodename=node8 --net=host -p 20008:20008 -p 20108:20108 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort9  -e nodename=node9 --net=host -p 20009:20009 -p 20109:20109 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort10  -e nodename=node10 --net=host -p 20010:20010 -p 20110:20110 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort11  -e nodename=node11 --net=host -p 20011:20011 -p 20111:20111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort12  -e nodename=node12 --net=host -p 20012:20012 -p 20112:20112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort13  -e nodename=node13 --net=host -p 20013:20013 -p 20113:20113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort14  -e nodename=node14 --net=host -p 20014:20014 -p 20114:20114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort15  -e nodename=node15 --net=host -p 20015:20015 -p 20115:20115 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

######    k8s-node02     #######
docker run --name terasort16  -e nodename=node16 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort17  -e nodename=node17 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort18  -e nodename=node18 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort19  -e nodename=node19 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort20  -e nodename=node20 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort21  -e nodename=node21 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort22  -e nodename=node22 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort23  -e nodename=node23 --net=host -p 20008:20008 -p 20108:20108 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort24  -e nodename=node24 --net=host -p 20009:20009 -p 20109:20109 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort25  -e nodename=node25 --net=host -p 20010:20010 -p 20110:20110 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort26  -e nodename=node26 --net=host -p 20011:20011 -p 20111:20111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort27  -e nodename=node27 --net=host -p 20012:20012 -p 20112:20112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort28  -e nodename=node28 --net=host -p 20013:20013 -p 20113:20113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort29  -e nodename=node29 --net=host -p 20014:20014 -p 20114:20114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort30  -e nodename=node30 --net=host -p 20015:20015 -p 20115:20115 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

######    k8s-node03     #######
docker run --name terasort31  -e nodename=node31 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort32  -e nodename=node32 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort33  -e nodename=node33 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort34  -e nodename=node34 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort35  -e nodename=node35 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort36  -e nodename=node36 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort37  -e nodename=node37 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort38  -e nodename=node38 --net=host -p 20008:20008 -p 20108:20108 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort39  -e nodename=node39 --net=host -p 20009:20009 -p 20109:20109 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort40  -e nodename=node40 --net=host -p 20010:20010 -p 20110:20110 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort41  -e nodename=node41 --net=host -p 20011:20011 -p 20111:20111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort42  -e nodename=node42 --net=host -p 20012:20012 -p 20112:20112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort43  -e nodename=node43 --net=host -p 20013:20013 -p 20113:20113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort44  -e nodename=node44 --net=host -p 20014:20014 -p 20114:20114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

######    k8s-node04     #######
docker run --name terasort45  -e nodename=node45 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort46  -e nodename=node46 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort47  -e nodename=node47 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort48  -e nodename=node48 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort49  -e nodename=node49 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort50  -e nodename=node50 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort51  -e nodename=node51 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort52  -e nodename=node52 --net=host -p 20008:20008 -p 20108:20108 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort53  -e nodename=node53 --net=host -p 20009:20009 -p 20109:20109 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort54  -e nodename=node54 --net=host -p 20010:20010 -p 20110:20110 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort55  -e nodename=node55 --net=host -p 20011:20011 -p 20111:20111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort56  -e nodename=node56 --net=host -p 20012:20012 -p 20112:20112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort57  -e nodename=node57 --net=host -p 20013:20013 -p 20113:20113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort58  -e nodename=node58 --net=host -p 20014:20014 -p 20114:20114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

######    k8s-node05     #######
docker run --name terasort59  -e nodename=node59 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort60  -e nodename=node60 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort61  -e nodename=node61 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort62  -e nodename=node62 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort63  -e nodename=node63 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort64  -e nodename=node64 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort65  -e nodename=node65 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort66  -e nodename=node66 --net=host -p 20008:20008 -p 20108:20108 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort67  -e nodename=node67 --net=host -p 20009:20009 -p 20109:20109 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort68  -e nodename=node68 --net=host -p 20010:20010 -p 20110:20110 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort69  -e nodename=node69 --net=host -p 20011:20011 -p 20111:20111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort70  -e nodename=node70 --net=host -p 20012:20012 -p 20112:20112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort71  -e nodename=node71 --net=host -p 20013:20013 -p 20113:20113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort72  -e nodename=node72 --net=host -p 20014:20014 -p 20114:20114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

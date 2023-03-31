######    k8s-node01     #######
docker stop terasort1
docker stop terasort2
docker stop terasort3
docker stop terasort4
docker stop terasort5
docker stop terasort6
docker stop terasort7
######    k8s-node02     #######
docker stop terasort8
docker stop terasort9
docker stop terasort10
docker stop terasort11
docker stop terasort12
docker stop terasort13

######    k8s-node01     #######
docker rm terasort1 
docker rm terasort2 
docker rm terasort3 
docker rm terasort4 
docker rm terasort5 
docker rm terasort6 
docker rm terasort7 
######    k8s-node02     #######
docker rm terasort8 
docker rm terasort9 
docker rm terasort10 
docker rm terasort11 
docker rm terasort12 
docker rm terasort13 

######    k8s-node01     #######
docker start terasort1 
docker start terasort2 
docker start terasort3 
docker start terasort4 
docker start terasort5 
docker start terasort6 
docker start terasort7 
######    k8s-node02     #######
docker start terasort8 
docker start terasort9 
docker start terasort10 
docker start terasort11 
docker start terasort12 
docker start terasort13 


######    k8s-node01     #######
docker run --name terasort1  -e nodename=node1 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10

docker run --name terasort2  -e nodename=node2 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort3  -e nodename=node3 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort4  -e nodename=node4 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort5  -e nodename=node5 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort6  -e nodename=node6 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort7  -e nodename=node7 --net=host -p 20007:20007 -p 20107:20107 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10

######    k8s-node02     #######
docker run --name terasort8  -e nodename=node8 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort9  -e nodename=node9 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort10  -e nodename=node10 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort11  -e nodename=node11 --net=host -p 20004:20004 -p 20104:20104 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort12  -e nodename=node12 --net=host -p 20005:20005 -p 20105:20105 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10
docker run --name terasort13  -e nodename=node13 --net=host -p 20006:20006 -p 20106:20106 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.10

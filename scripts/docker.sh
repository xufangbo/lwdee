docker stop terasort1
docker stop terasort2
docker stop terasort3
docker stop terasort4
docker stop terasort5
docker stop terasort6

docker rm terasort1 
docker rm terasort2 
docker rm terasort3 
docker rm terasort4 
docker rm terasort5 
docker rm terasort6 


######    k8s-node01     #######
docker run --name terasort1  -e nodename=node1 --net=host -p 2101:2101 -p 2111:2111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

docker run --name terasort2  -e nodename=node2 --net=host -p 2102:2102 -p 2112:2112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

######    k8s-node02     #######
docker run --name terasort3  -e nodename=node3 --net=host -p 2103:2103 -p 2113:2113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

######    k8s-node03     #######
docker run --name terasort4  -e nodename=node4 --net=host -p 2104:2104 -p 2114:2114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

######    k8s-node04     #######
docker run --name terasort5  -e nodename=node5 --net=host -p 2105:2105 -p 2115:2115 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

######    k8s-node05     #######
docker run --name terasort6  -e nodename=node6 --net=host -p 2106:2106 -p 2116:2116 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12

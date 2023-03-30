######    k8s-master     #######
docker stop terasort1
######    k8s-node01     #######
docker stop terasort2
docker stop terasort3

######    k8s-master     #######
docker rm terasort1 
######    k8s-node01     #######
docker rm terasort2 
docker rm terasort3 

######    k8s-master     #######
docker start terasort1 
######    k8s-node01     #######
docker start terasort2 
docker start terasort3 

######    k8s-master     #######
docker run --name terasort1  -e nodename=node1 --net=host -p 30001:30001 -p 30101:30101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
######    k8s-node01     #######
docker run --name terasort2  -e nodename=node2 --net=host -p 30001:30001 -p 30101:30101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
docker run --name terasort3  -e nodename=node3 --net=host -p 30002:30002 -p 30102:30102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4

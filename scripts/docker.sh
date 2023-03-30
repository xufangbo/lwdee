######    localhost     #######
docker stop terasort1
docker stop terasort2
docker stop terasort3

docker rm terasort1 
docker rm terasort2 
docker rm terasort3 

docker start terasort1 
docker start terasort2 
docker start terasort3 

docker run --name terasort1  -e nodename=node1 --net=host -p 20001:20001 -p 20101:20101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort2  -e nodename=node2 --net=host -p 20002:20002 -p 20102:20102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8
docker run --name terasort3  -e nodename=node3 --net=host -p 20003:20003 -p 20103:20103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.8

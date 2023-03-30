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

docker run --name terasort1  -e nodename=node1 --net=host -p 30001:30001 -p 30101:30101 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
docker run --name terasort2  -e nodename=node2 --net=host -p 30002:30002 -p 30102:30102 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
docker run --name terasort3  -e nodename=node3 --net=host -p 30003:30003 -p 30103:30103 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4

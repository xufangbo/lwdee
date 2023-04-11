docker stop terasort1
docker stop terasort2
docker stop terasort3
docker stop terasort4

docker rm terasort1 
docker rm terasort2 
docker rm terasort3 
docker rm terasort4 



ssh root@localhost "docker run --name terasort1  -e nodename=node1 --net=host -p 3101:3101 -p 3111:3111 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@localhost "docker run --name terasort2  -e nodename=node2 --net=host -p 3102:3102 -p 3112:3112 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@localhost "docker run --name terasort3  -e nodename=node3 --net=host -p 3103:3103 -p 3113:3113 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@localhost "docker run --name terasort4  -e nodename=node4 --net=host -p 3104:3104 -p 3114:3114 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


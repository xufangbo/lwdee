docker stop terasort1
docker stop terasort2

docker rm terasort1 
docker rm terasort2 



ssh root@localhost "docker run --name terasort1  -e nodename=node1 --net=host -p 2701:2701 -p 2711:2711 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@localhost "docker run --name terasort2  -e nodename=node2 --net=host -p 2702:2702 -p 2712:2712 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


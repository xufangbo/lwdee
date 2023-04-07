docker stop terasort1
docker stop terasort2
docker stop terasort3

docker rm terasort1 
docker rm terasort2 
docker rm terasort3 



ssh root@localhost "docker run --name terasort1  -e nodename=node1 --net=host -p 7801:7801 -p 7811:7811 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@localhost "docker run --name terasort2  -e nodename=node2 --net=host -p 7802:7802 -p 7812:7812 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@localhost "docker run --name terasort3  -e nodename=node3 --net=host -p 7803:7803 -p 7813:7813 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


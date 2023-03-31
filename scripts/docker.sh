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



ssh root@k8s-node01 "docker run --name terasort1  -e nodename=node1 --net=host -p 4601:4601 -p 4611:4611 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@k8s-node01 "docker run --name terasort2  -e nodename=node2 --net=host -p 4602:4602 -p 4612:4612 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node02 "docker run --name terasort3  -e nodename=node3 --net=host -p 4603:4603 -p 4613:4613 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node03 "docker run --name terasort4  -e nodename=node4 --net=host -p 4604:4604 -p 4614:4614 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node04 "docker run --name terasort5  -e nodename=node5 --net=host -p 4605:4605 -p 4615:4615 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node05 "docker run --name terasort6  -e nodename=node6 --net=host -p 4606:4606 -p 4616:4616 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


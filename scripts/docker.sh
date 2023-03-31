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



ssh root@k8s-node01 "docker run --name terasort1  -e nodename=node1 --net=host -p 4301:4301 -p 4311:4311 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@k8s-node01 "docker run --name terasort2  -e nodename=node2 --net=host -p 4302:4302 -p 4312:4312 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node02 "docker run --name terasort3  -e nodename=node3 --net=host -p 4303:4303 -p 4313:4313 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node03 "docker run --name terasort4  -e nodename=node4 --net=host -p 4304:4304 -p 4314:4314 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node04 "docker run --name terasort5  -e nodename=node5 --net=host -p 4305:4305 -p 4315:4315 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node05 "docker run --name terasort6  -e nodename=node6 --net=host -p 4306:4306 -p 4316:4316 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


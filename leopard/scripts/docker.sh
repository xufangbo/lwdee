docker run --name leopard   --net=host -v /home/kevin/gitee/leopard/app:/home/leopard/app -v /home/kevin/gitee/leopard/log:/home/leopard/log -v /home/kevin/gitee/leopard/config:/home/leopard/config -p 4703:4703   -d registry.cn-beijing.aliyuncs.com/xufangbo/leopard:v1.0.8
docker run --name leopard-client --net=host -v /home/kevin/gitee/leopard/client:/home/leopard/client -v /home/kevin/gitee/leopard/log:/home/leopard/log -v /home/kevin/gitee/leopard/config:/home/leopard/config -d registry.cn-beijing.aliyuncs.com/xufangbo/leopard-client:v1.0.8

docker exec -it leopard /bin/bash
docker exec -it leopard-client /bin/bash

docker stop leopard
docker rm leopard

docker logs leopard

docker stop leopard-client
docker rm leopard-client

docker logs leopard-client


ssh root@k8s-node01 "docker run --name terasort1  -e nodename=node1 --net=host -p 4701:4701 -p 4711:4711 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@k8s-node01 "docker run --name terasort2  -e nodename=node2 --net=host -p 4702:4702 -p 4712:4712 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node02 "docker run --name terasort3  -e nodename=node3 --net=host -p 4703:4703 -p 4713:4713 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node03 "docker run --name terasort4  -e nodename=node4 --net=host -p 4704:4704 -p 4714:4714 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node04 "docker run --name terasort5  -e nodename=node5 --net=host -p 4705:4705 -p 4715:4715 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@k8s-node05 "docker run --name terasort6  -e nodename=node6 --net=host -p 4706:4706 -p 4716:4716 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@k8s-master "docker stop dsc1"
ssh root@k8s-node01 "docker stop dsc2"
ssh root@k8s-node02 "docker stop dsc3"
ssh root@k8s-node03 "docker stop dsc4"
ssh root@k8s-node04 "docker stop dsc5"
ssh root@k8s-node05 "docker stop dsc6"


ssh root@k8s-master "docker rm dsc1"
ssh root@k8s-node01 "docker rm dsc2"
ssh root@k8s-node02 "docker rm dsc3"
ssh root@k8s-node03 "docker rm dsc4"
ssh root@k8s-node04 "docker rm dsc5"
ssh root@k8s-node05 "docker rm dsc6"


ssh root@k8s-master "docker start dsc1"
ssh root@k8s-node01 "docker start dsc2"
ssh root@k8s-node02 "docker start dsc3"
ssh root@k8s-node03 "docker start dsc4"
ssh root@k8s-node04 "docker start dsc5"
ssh root@k8s-node05 "docker start dsc6"


docker stop dsc1
docker stop dsc2
docker stop dsc3
docker stop dsc4
docker stop dsc5
docker stop dsc6


docker rm dsc1
docker rm dsc2
docker rm dsc3
docker rm dsc4
docker rm dsc5
docker rm dsc6


ssh root@k8s-master "docker run --name dsc1  -e nodename=node1 --net=host -p 1101:1101 -p 1111:1111 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


ssh root@k8s-node01 "docker run --name dsc2  -e nodename=node2 --net=host -p 1102:1102 -p 1112:1112 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node02 "docker run --name dsc3  -e nodename=node3 --net=host -p 1103:1103 -p 1113:1113 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node03 "docker run --name dsc4  -e nodename=node4 --net=host -p 1104:1104 -p 1114:1114 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node04 "docker run --name dsc5  -e nodename=node5 --net=host -p 1105:1105 -p 1115:1115 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node05 "docker run --name dsc6  -e nodename=node6 --net=host -p 1106:1106 -p 1116:1116 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


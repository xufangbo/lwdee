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

docker start dsc1
docker start dsc2
docker start dsc3
docker start dsc4
docker start dsc5
docker start dsc6

ssh root@k8s-master "docker run --name dsc1  -e nodename=node1 --net=host -p 5101:5101 -p 5111:5111 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


ssh root@k8s-node01 "docker run --name dsc2  -e nodename=node2 --net=host -p 5102:5102 -p 5112:5112 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node02 "docker run --name dsc3  -e nodename=node3 --net=host -p 5103:5103 -p 5113:5113 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node03 "docker run --name dsc4  -e nodename=node4 --net=host -p 5104:5104 -p 5114:5114 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node04 "docker run --name dsc5  -e nodename=node5 --net=host -p 5105:5105 -p 5115:5115 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node05 "docker run --name dsc6  -e nodename=node6 --net=host -p 5106:5106 -p 5116:5116 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


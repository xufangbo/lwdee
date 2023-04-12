ssh root@k8s-master "docker stop dsc1"
ssh root@k8s-node01 "docker stop dsc2"
ssh root@k8s-node02 "docker stop dsc3"
ssh root@k8s-node03 "docker stop dsc4"

ssh root@k8s-master "docker rm dsc1"
ssh root@k8s-node01 "docker rm dsc2"
ssh root@k8s-node02 "docker rm dsc3"
ssh root@k8s-node03 "docker rm dsc4"

ssh root@k8s-master "docker start dsc1"
ssh root@k8s-node01 "docker start dsc2"
ssh root@k8s-node02 "docker start dsc3"
ssh root@k8s-node03 "docker start dsc4"



ssh root@k8s-master "docker run --name dsc1  -e nodename=node1 --net=host -p 4401:4401 -p 4411:4411 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


ssh root@k8s-node01 "docker run --name dsc2  -e nodename=node2 --net=host -p 4402:4402 -p 4412:4412 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node02 "docker run --name dsc3  -e nodename=node3 --net=host -p 4403:4403 -p 4413:4413 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node03 "docker run --name dsc4  -e nodename=node4 --net=host -p 4404:4404 -p 4414:4414 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


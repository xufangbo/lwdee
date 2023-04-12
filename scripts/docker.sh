docker stop dsc1
docker stop dsc2
docker stop dsc3
docker stop dsc4

docker rm dsc1 
docker rm dsc2 
docker rm dsc3 
docker rm dsc4 



ssh root@k8s-master "docker run --name dsc0  -e nodename=node0 --net=host -p 1301:1301 -p 1311:1311 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


ssh root@k8s-node01 "docker run --name dsc1  -e nodename=node1 --net=host -p 1302:1302 -p 1312:1312 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node02 "docker run --name dsc2  -e nodename=node2 --net=host -p 1303:1303 -p 1313:1313 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"
ssh root@k8s-node03 "docker run --name dsc3  -e nodename=node3 --net=host -p 1304:1304 -p 1314:1314 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8"


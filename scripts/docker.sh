docker stop dsc1
docker stop dsc2
docker stop dsc3
docker stop dsc4

docker rm dsc1 
docker rm dsc2 
docker rm dsc3 
docker rm dsc4 



ssh root@localhost "docker run --name dsc1  -e nodename=node1 --net=host -p 4401:4401 -p 4411:4411 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.18"


ssh root@localhost "docker run --name dsc2  -e nodename=node2 --net=host -p 4402:4402 -p 4412:4412 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.18"
ssh root@localhost "docker run --name dsc3  -e nodename=node3 --net=host -p 4403:4403 -p 4413:4413 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.18"
ssh root@localhost "docker run --name dsc4  -e nodename=node4 --net=host -p 4404:4404 -p 4414:4414 -v /home/kevin/git/lwdee/build/app:/home/dsc/app -v /home/kevin/git/lwdee/log:/home/dsc/log -v /home/kevin/git/lwdee/data:/home/dsc/data -v /home/kevin/git/lwdee/config:/home/dsc/config -d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.18"


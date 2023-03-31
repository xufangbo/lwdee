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

ssh root@k8s-node01 "docker run --name terasort1  -e nodename=node1 --net=host -p 3901:3901 -p 3911:3911 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"


ssh root@k8s-node01 "docker run --name terasort2  -e nodename=node2 --net=host -p 3902:3902 -p 3912:3912 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"
ssh root@k8s-node02 "docker run --name terasort3  -e nodename=node3 --net=host -p 3903:3903 -p 3913:3913 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"
ssh root@k8s-node03 "docker run --name terasort4  -e nodename=node4 --net=host -p 3904:3904 -p 3914:3914 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"
ssh root@k8s-node04 "docker run --name terasort5  -e nodename=node5 --net=host -p 3905:3905 -p 3915:3915 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"
ssh root@k8s-node05 "docker run --name terasort6  -e nodename=node6 --net=host -p 3906:3906 -p 3916:3916 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.17"


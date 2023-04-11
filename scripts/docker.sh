docker stop terasort1
docker stop terasort2
docker stop terasort3
docker stop terasort4

docker rm terasort1 
docker rm terasort2 
docker rm terasort3 
docker rm terasort4 



ssh root@localhost "docker run --name terasort1  -e nodename=node1 --net=host -p 2901:2901 -p 2911:2911 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


ssh root@localhost "docker run --name terasort2  -e nodename=node2 --net=host -p 2902:2902 -p 2912:2912 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@localhost "docker run --name terasort3  -e nodename=node3 --net=host -p 2903:2903 -p 2913:2913 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"
ssh root@localhost "docker run --name terasort4  -e nodename=node4 --net=host -p 2904:2904 -p 2914:2914 -v /home/kevin/git/lwdee/log:/home/terasort/log -v /home/kevin/git/lwdee/data:/home/terasort/data -v /home/kevin/git/lwdee/config:/home/terasort/config -d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.18"


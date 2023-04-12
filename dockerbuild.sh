app=dsc

docker stop $app
docker rm $app
docker image rm $app
docker build -t $app .
# docker run --name $app -p 16501:16501 -e nodename=node3 -v /home/kevin/git/lwdee/log1:/home/log -d $app 

# docker exec -it  terasort2 /bin/bash

# # 阿里与云个人镜像：公网
# docker login --username=xufa****@***.com registry.cn-beijing.aliyuncs.com

docker tag $app registry.cn-beijing.aliyuncs.com/xufangbo/$app:v1.0.1
docker push registry.cn-beijing.aliyuncs.com/xufangbo/$app:v1.0.1

# docker pull registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.18

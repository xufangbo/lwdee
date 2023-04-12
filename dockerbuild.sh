app=dsc
version=v1.0.8

docker stop $app
docker rm $app
docker image rm $app
docker build -t $app . #--no-cache
# docker run --name $app -p 16501:16501 -e nodename=node3 -v /home/kevin/git/lwdee/log1:/home/log -d $app 

# docker exec -it  terasort2 /bin/bash

# # 阿里与云个人镜像：公网
# docker login --username=xufa****@***.com registry.cn-beijing.aliyuncs.com

docker tag $app registry.cn-beijing.aliyuncs.com/xufangbo/$app:$version
docker push registry.cn-beijing.aliyuncs.com/xufangbo/$app:$version

# docker pull registry.cn-beijing.aliyuncs.com/xufangbo/dsc:v1.0.8

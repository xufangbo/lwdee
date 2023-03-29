app=terasort
# go build -o $app --ldflags "-extldflags -static"

# mkdir -p ./lib/std
# cp -f /usr/lib/x86_64-linux-gnu/libstdc++.so* ./lib/std
# cp -f /usr/lib/x86_64-linux-gnu/libc.so* ./lib/std


docker stop $app
docker rm $app
docker image rm $app
docker build -t $app .
# docker run --name $app -p 16501:16501 -e nodename=node3 -v /home/kevin/git/lwdee/log1:/home/log -d $app 

# # 阿里与云个人镜像：公网
# docker tag terasort registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.0
# docker push registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.0

# docker exec -it  terasort2 /bin/bash
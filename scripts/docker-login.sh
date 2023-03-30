docker login --username=xufangbo@hotmail.com registry.cn-beijing.aliyuncs.com

docker tag terasort registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.5
docker push registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.5

docker pull registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.5
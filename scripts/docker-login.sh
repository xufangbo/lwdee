docker login --username=xufangbo@hotmail.com registry.cn-beijing.aliyuncs.com

docker tag terasort registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
docker push registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4

docker pull registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.4
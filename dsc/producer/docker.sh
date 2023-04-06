docker run --name dscproducer  -e nodename=node1 --net=host  -d registry.cn-beijing.aliyuncs.com/xufangbo/dscproducer:v1.0.4

docker stop dscproducer
docker rm dscproducer

docker logs dscproducer


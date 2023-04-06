ubuntu可以通过sudo apt-get intall librdkafka++-dev来安装不过版本是1.3的比较老，可以使用下面的地址下载编译后安装
https://github.com/confluentinc/librdkafka/releases

./configure --prefix /home/kevin/tmp/librdkafka
make
make install

安装完之后复制so文件到项目中会有引用个错误，所以需要使用下面的脚本复制到/usr/local/lib下

cd /home/kevin/tmp/librdkafka
sudo cp -rf ./* /usr/local/lib/
ls /usr/local/lib/

kafka 部署好了，在 10.180.98.131,132,133 上，订阅端口是9092，
你找个手册参考一下相关命令和使用的方式哈。创建了一个topic ：test。可以手工自己发送消息上去。

cd /home/apps/kafka/kafka_2.11-2.1.1/bin
./kafka-consumer-groups.sh --bootstrap-server  10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092 --list
./kafka-console-consumer.sh --bootstrap-server 10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092 --group topic_lwdee --topic test --from-beginning
./kafka-console-producer.sh --broker-list 10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092  --topic test

# 创建分区
./kafka-topics.sh --zookeeper 10.180.98.131:2181,10.180.98.132:2181,10.180.98.133:2181 --create --topic test-topic --partitions 3 --replication-factor 3
# 查看分区
./kafka-topics.sh --zookeeper 10.180.98.131:2181,10.180.98.132:2181,10.180.98.133:2181 -list
./kafka-topics.sh --zookeeper 10.180.98.131:2181,10.180.98.132:2181,10.180.98.133:2181 --describe --topic test

./build/app -C -t test -b "10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092" -p 0
./build/producer
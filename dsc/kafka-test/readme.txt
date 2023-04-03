sudo mkdir -p /usr/local/lib/rdkafka
sudo cp -rf ./* /usr/local/lib/rdkafka
ls /usr/local/lib/rdkafka

kafka 部署好了，在 10.180.98.131,132,133 上，订阅端口是9092，
你找个手册参考一下相关命令和使用的方式哈。创建了一个topic ：test。可以手工自己发送消息上去。

cd /home/apps/kafka/kafka_2.11-2.1.1/bin
./kafka-consumer-groups.sh --bootstrap-server  10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092 --list
./kafka-console-consumer.sh --bootstrap-server 10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092 --group topic_lwdee --topic test --from-beginning
./kafka-console-producer.sh --broker-list 10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092  --topic test

./build/app -C -t test -b "10.180.98.131:9092,10.180.98.132:9092,10.180.98.133:9092" -p 1
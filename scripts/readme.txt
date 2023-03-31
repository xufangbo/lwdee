1. 修改{lwdee-home}/config/terasort.json中的并发数
2. 执行{lwdee-home}/scripts/conf.js脚本生成{lwdee-home}/config/conf.json，修改端口号
3. k8s-node1-5执行{lwdee-home}/scripts/docker.sh中的docker stop及docker rm
4. sh /home/kevin/git/lwdee/scripts/copy.sh ,同步配置文件
5. 如果需要，更新新的镜像
6. 启动worker: 执行{lwdee-home}/scripts/docker.sh中的docker run创建容器(node2 - node 6)
7. 启动driver: 执行{lwdee-home}/scripts/docker.sh中的docker run创建容器(node1)
8. 在k8s-node1上执行docker logs terasort1查看执行情况
9. sh /home/kevin/git/lwdee/scripts/logpull.sh 获取日志（需要修改这个脚本中的dir变量）
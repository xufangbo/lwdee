

ssh root@k8s-master "docker start leopard"
ssh root@k8s-node01 "docker start leopard-client"
ssh root@k8s-node02 "docker start leopard-client"
ssh root@k8s-node03 "docker start leopard-client"
ssh root@k8s-node04 "docker start leopard-client"
ssh root@k8s-node05 "docker start leopard-client"


ssh root@k8s-node01 "docker start leopard-client2"
ssh root@k8s-node02 "docker start leopard-client2"
ssh root@k8s-node03 "docker start leopard-client2"
ssh root@k8s-node04 "docker start leopard-client2"
ssh root@k8s-node05 "docker start leopard-client2"


ssh root@k8s-node01 "docker stop leopard-client"
ssh root@k8s-node01 "docker stop leopard-client2"
ssh root@k8s-node02 "docker stop leopard-client"
ssh root@k8s-node02 "docker stop leopard-client2"
ssh root@k8s-node03 "docker stop leopard-client"
ssh root@k8s-node03 "docker stop leopard-client2"
ssh root@k8s-node04 "docker stop leopard-client"
ssh root@k8s-node04 "docker stop leopard-client2"
ssh root@k8s-node05 "docker stop leopard-client"
ssh root@k8s-node05 "docker stop leopard-client2"
ssh root@k8s-master "docker stop leopard"



tail -fn 50 /home/kevin/gitee/leopard/log/leopard-
tail -fn 50 /home/kevin/gitee/leopard/log/client-




ssh root@k8s-master "rm -rf /home/kevin/gitee/leopard/log/*"
ssh root@k8s-node01 "rm -rf /home/kevin/gitee/leopard/log/*"
ssh root@k8s-node02 "rm -rf /home/kevin/gitee/leopard/log/*"
ssh root@k8s-node03 "rm -rf /home/kevin/gitee/leopard/log/*"
ssh root@k8s-node04 "rm -rf /home/kevin/gitee/leopard/log/*"
ssh root@k8s-node05 "rm -rf /home/kevin/gitee/leopard/log/*"



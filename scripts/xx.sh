
ssh root@k8s-node01 "docker ps"
ssh root@k8s-node02 "docker ps"
ssh root@k8s-node03 "docker ps"
ssh root@k8s-node04 "docker ps"
ssh root@k8s-node05 "docker ps"


ssh root@k8s-node01 "docker ps -l"
ssh root@k8s-node02 "docker ps -l"
ssh root@k8s-node03 "docker ps -l"
ssh root@k8s-node04 "docker ps -l"
ssh root@k8s-node05 "docker ps -l"



ssh root@k8s-node01 "docker logs terasort1"

ssh root@k8s-node02 "docker logs terasort3"


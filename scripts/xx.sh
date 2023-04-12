
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


ssh root@k8s-master "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node01 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node02 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node03 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node04 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node05 "rm -rf /home/kevin/git/lwdee/log/*"



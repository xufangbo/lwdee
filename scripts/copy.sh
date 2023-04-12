
tail -fn 50 /home/kevin/git/lwdee/log/dsc-2023-04-12.log
docker logs -f dsc


scp /home/kevin/git/lwdee/build/app root@k8s-master:/home/kevin/git/lwdee/build/
scp /home/kevin/git/lwdee/build/app root@k8s-node01:/home/kevin/git/lwdee/build/
scp /home/kevin/git/lwdee/build/app root@k8s-node02:/home/kevin/git/lwdee/build/
scp /home/kevin/git/lwdee/build/app root@k8s-node03:/home/kevin/git/lwdee/build/
scp /home/kevin/git/lwdee/build/app root@k8s-node04:/home/kevin/git/lwdee/build/
scp /home/kevin/git/lwdee/build/app root@k8s-node05:/home/kevin/git/lwdee/build/


scp /home/kevin/git/lwdee/config/dsc.json root@k8s-master:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/dsc.json root@k8s-node01:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/dsc.json root@k8s-node02:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/dsc.json root@k8s-node03:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/dsc.json root@k8s-node04:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/dsc.json root@k8s-node05:/home/kevin/git/lwdee/config/

scp /home/kevin/git/lwdee/config/conf.json root@k8s-master:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/conf.json root@k8s-node01:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/conf.json root@k8s-node02:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/conf.json root@k8s-node03:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/conf.json root@k8s-node04:/home/kevin/git/lwdee/config/
scp /home/kevin/git/lwdee/config/conf.json root@k8s-node05:/home/kevin/git/lwdee/config/

ssh root@k8s-master "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node01 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node02 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node03 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node04 "rm -rf /home/kevin/git/lwdee/log/*"
ssh root@k8s-node05 "rm -rf /home/kevin/git/lwdee/log/*"




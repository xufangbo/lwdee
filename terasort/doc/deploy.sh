# scp /home/kevin/git/lwdee/build/app root@k8s-master:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/build/app root@k8s-node01:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/build/app root@k8s-node02:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/build/app root@k8s-node03:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/build/app root@k8s-node04:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/build/app root@k8s-node05:/home/data/terasort/app/

# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-master:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-node01:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-node02:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-node03:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-node04:/home/data/terasort/app/
# scp /home/kevin/git/lwdee/test/node_conf.json root@k8s-node05:/home/data/terasort/app/


cd /home/kevin/git
rm -rf lwdee.tar.gz
tar -zcvf lwdee.tar.gz lwdee

ssh root@k8s-master "rm -rf /home/data/terasort/lwdee*"
ssh root@k8s-node01 "rm -rf /home/data/terasort/lwdee*"
ssh root@k8s-node02 "rm -rf /home/data/terasort/lwdee*"
ssh root@k8s-node03 "rm -rf /home/data/terasort/lwdee*"
ssh root@k8s-node04 "rm -rf /home/data/terasort/lwdee*"
ssh root@k8s-node05 "rm -rf /home/data/terasort/lwdee*"

scp /home/kevin/git/lwdee.tar.gz root@k8s-master:/home/data/terasort/
scp /home/kevin/git/lwdee.tar.gz root@k8s-node01:/home/data/terasort/
scp /home/kevin/git/lwdee.tar.gz root@k8s-node02:/home/data/terasort/
scp /home/kevin/git/lwdee.tar.gz root@k8s-node03:/home/data/terasort/
scp /home/kevin/git/lwdee.tar.gz root@k8s-node04:/home/data/terasort/
scp /home/kevin/git/lwdee.tar.gz root@k8s-node05:/home/data/terasort/


unzip lwdee.tar.gz
cd /home/data/terasort/lwdee/build
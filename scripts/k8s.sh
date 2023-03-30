
ssh root@k8s-master "rm -rf /home/kevin/git/lwdee/data/*"
ssh root@k8s-node01 "rm -rf /home/kevin/git/lwdee/data/*"
ssh root@k8s-node02 "rm -rf /home/kevin/git/lwdee/data/*"
ssh root@k8s-node03 "rm -rf /home/kevin/git/lwdee/data/*"
ssh root@k8s-node04 "rm -rf /home/kevin/git/lwdee/data/*"
ssh root@k8s-node05 "rm -rf /home/kevin/git/lwdee/data/*"

ssh root@k8s-master "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"
ssh root@k8s-node01 "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"
ssh root@k8s-node02 "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"
ssh root@k8s-node03 "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"
ssh root@k8s-node04 "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"
ssh root@k8s-node05 "cp /home/data/terasort/data-input.dat /home/kevin/git/lwdee/data/"


ssh root@k8s-master "ls /home/kevin/git/lwdee/data"
ssh root@k8s-node01 "ls /home/kevin/git/lwdee/data"
ssh root@k8s-node02 "ls /home/kevin/git/lwdee/data"
ssh root@k8s-node03 "ls /home/kevin/git/lwdee/data"
ssh root@k8s-node04 "ls /home/kevin/git/lwdee/data"
ssh root@k8s-node05 "ls /home/kevin/git/lwdee/data"
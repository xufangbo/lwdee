ssh root@k8s-master "rm -rf /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node01 "rm -rf /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node02 "rm -rf /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node03 "rm -rf /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node04 "rm -rf /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node05 "rm -rf /home/kevin/git/lwdee/data/data-input.dat"


ssh root@k8s-master "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node01 "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node02 "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node03 "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node04 "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"
ssh root@k8s-node05 "mv /home/data/datasets/allpart/all-part-records-00000 /home/kevin/git/lwdee/data/data-input.dat"


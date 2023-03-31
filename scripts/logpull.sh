dir=48


ssh root@k8s-node01 "rm -rf node1-driver.log && docker logs terasort1 > node1-driver.log"
ssh root@k8s-node01 "rm -rf node2.log && docker logs terasort2 > node2.log"
ssh root@k8s-node02 "rm -rf node3.log && docker logs terasort3 > node3.log"
ssh root@k8s-node03 "rm -rf node4.log && docker logs terasort4 > node4.log"
ssh root@k8s-node04 "rm -rf node5.log && docker logs terasort5 > node5.log"
ssh root@k8s-node05 "rm -rf node6.log && docker logs terasort6 > node6.log"


scp root@k8s-node01:/root/node1-driver.log ../prod-log/${dir}
scp root@k8s-node01:/root/node2.log ../prod-log/${dir}
scp root@k8s-node02:/root/node3.log ../prod-log/${dir}
scp root@k8s-node03:/root/node4.log ../prod-log/${dir}
scp root@k8s-node04:/root/node5.log ../prod-log/${dir}
scp root@k8s-node05:/root/node6.log ../prod-log/${dir}
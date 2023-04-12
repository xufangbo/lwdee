const fs = require('fs');

// let workers = [
//     { "name": "k8s-master", "ip": "10.180.98.130" },
//     { "name": "k8s-node01", "ip": "10.180.98.131" },
//     { "name": "k8s-node02", "ip": "10.180.98.132" },
//     { "name": "k8s-node03", "ip": "10.180.98.133" },
//     { "name": "k8s-node04", "ip": "10.180.98.134" },
//     { "name": "k8s-node05", "ip": "10.180.98.135" }
// ];

let workers = [
    { "name": "k8s-master", "ip": "127.0.0.1" },
    { "name": "k8s-node01", "ip": "127.0.0.1" },
    { "name": "k8s-node02", "ip": "127.0.0.1" },
    { "name": "k8s-node03", "ip": "127.0.0.1" },
];

let imgVersion = "v1.0.8";
let fileName = "/home/kevin/git/lwdee/config/conf.json";

let json = fs.readFileSync(fileName);
let db = JSON.parse(json);
let port = db.port > 800 ? 100 : db.port + 10;

let routerInfos = workers.map(x => {
    let i = workers.indexOf(x);
    let portTail = (i + 1).toString();
    return { "worker": x.name, "nid": i + 1, "ip": x.ip, "dport": parseInt(port + portTail), "mport": parseInt((port + 1) + portTail) };
});

let conf = {
    "node_amount": workers.length,
    "port": port
};

for (var i in routerInfos) {
    let routerInfo = routerInfos[i];
    conf["node" + routerInfo.nid] = {
        "nodeId": routerInfo.nid,
        // "ip": routerInfo.ip,
        "ip": "127.0.0.1",
        "dataPort": routerInfo.dport,
        "msgPort": routerInfo.mport,
        "routeInfo": routerInfos
    };
}

json = JSON.stringify(conf, null, "  ");

fs.writeFileSync(fileName, json);

//===============================

// docker run --name $app -p 16501:16501 -e nodename=node3 -v /home/kevin/git/lwdee/log1:/home/log -d $app 

let deployScripts = [];
let preWorker;
for (var i in routerInfos) {
    let router = routerInfos[i];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    deployScripts.push(`ssh root@${router.worker} "docker stop dsc${router.nid}"`);
}
deployScripts.push("");
for (var i in routerInfos) {
    let router = routerInfos[i];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    deployScripts.push(`ssh root@${router.worker} "docker rm dsc${router.nid}"`);
}
deployScripts.push("");
for (var i in routerInfos) {
    let router = routerInfos[i];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    deployScripts.push(`ssh root@${router.worker} "docker start dsc${router.nid}"`);
}

deployScripts.push("");
deployScripts.push("");
deployScripts.push("");

// for (var ri in routerInfos) {
//     let router = routerInfos[ri];
//     if (router.worker != preWorker) {
//         preWorker = router.worker;
//     }
//     deployScripts.push(`docker start dsc${router.nid} `);
// }
// deployScripts.push("");

for (var i in routerInfos) {
    let router = routerInfos[i];

    deployScripts.push(`ssh root@${router.worker} "docker run --name dsc${router.nid}  -e nodename=node${router.nid} --net=host ` +
        `-p ${router.dport}:${router.dport} -p ${router.mport}:${router.mport} ` +
        `-v /home/kevin/git/lwdee/build/app:/home/dsc/app ` +
        `-v /home/kevin/git/lwdee/log:/home/dsc/log ` +
        `-v /home/kevin/git/lwdee/data:/home/dsc/data ` +
        `-v /home/kevin/git/lwdee/config:/home/dsc/config ` +
        `-d registry.cn-beijing.aliyuncs.com/xufangbo/dsc:${imgVersion}"`);

    if (i == 0) {
        deployScripts.push("");
        deployScripts.push("");
    }
}
deployScripts.push("");
deployScripts.push("");

// for (var i in routerInfos) {
//     let router = routerInfos[i];
//     let fname = i == 0 ?  `node${router.nid}-driver.log` :  `node${router.nid}.log`;
//     deployScripts.push(`ssh root@${router.worker} "rm -rf ${fname} && docker logs dsc${router.nid} > ${fname}"`);
// }
// deployScripts.push("");
// deployScripts.push("");

fileName = "/home/kevin/git/lwdee/scripts/docker.sh";
fs.writeFileSync(fileName, deployScripts.join("\r\n"));

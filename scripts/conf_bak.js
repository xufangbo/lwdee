const fs = require('fs');

/**
 * 生成配置文件
 */
let workers = [
    { "name": "k8s-node01", "ip": "10.180.98.131" },
    { "name": "k8s-node01", "ip": "10.180.98.131" },
    { "name": "k8s-node02", "ip": "10.180.98.132" },
    { "name": "k8s-node03", "ip": "10.180.98.133" },
    { "name": "k8s-node04", "ip": "10.180.98.134" },
    { "name": "k8s-node05", "ip": "10.180.98.135" }
];

// let workers = [
//     { "name": "k8s-node01", "ip": "10.180.98.131" },
//     { "name": "k8s-node01", "ip": "10.180.98.131" },
//     { "name": "k8s-node02", "ip": "10.180.98.132" }];

// let workers = [
//     { "name": "localhost", "ip": "127.0.0.1" },
//     { "name": "localhost", "ip": "127.0.0.1" },
//     { "name": "localhost", "ip": "127.0.0.1" }
// ];

let fileName = "/home/kevin/git/lwdee/config/conf.json";

// let json = fs.readFileSync(fileName);
// let db = JSON.parse(json);
// let port = db.port > 200 ? 100 : db.port + 2;
let port = 210;

let routerInfos = workers.map(x => {
    let i = workers.indexOf(x);
    let portTail = (i + 1).toString();
    return { "worker": x.name, "nid": i + 1, "ip": x.ip, "dport": parseInt(port + portTail), "mport": parseInt((port + 1) + portTail) };
});

let conf = {
    "node_amount": workers.length,
    "port": port
};

for (var ri in routerInfos) {
    let routerInfo = routerInfos[ri];
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
for (var ri in routerInfos) {
    let router = routerInfos[ri];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    deployScripts.push(`docker stop terasort${router.nid}`);
}
deployScripts.push("");
for (var ri in routerInfos) {
    let router = routerInfos[ri];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    deployScripts.push(`docker rm terasort${router.nid} `);
}

deployScripts.push("");

// for (var ri in routerInfos) {
//     let router = routerInfos[ri];
//     if (router.worker != preWorker) {
//         preWorker = router.worker;
//     }
//     deployScripts.push(`docker start terasort${router.nid} `);
// }
// deployScripts.push("");

for (var ri in routerInfos) {
    let router = routerInfos[ri];
    if (router.worker != preWorker) {
        deployScripts.push("");
        deployScripts.push(`######    ${router.worker}     #######`);
        preWorker = router.worker;
    }
    deployScripts.push(`docker run --name terasort${router.nid}  -e nodename=node${router.nid} --net=host ` +
        `-p ${router.dport}:${router.dport} -p ${router.mport}:${router.mport} ` +
        `-v /home/kevin/git/lwdee/log:/home/terasort/log ` +
        `-v /home/kevin/git/lwdee/data:/home/terasort/data ` +
        `-v /home/kevin/git/lwdee/config:/home/terasort/config ` +
        `-d registry.cn-beijing.aliyuncs.com/xufangbo/terasort:v1.0.12`);
    if (ri == 0) {
        deployScripts.push("");
    }
}
deployScripts.push("");

fileName = "/home/kevin/git/lwdee/scripts/docker.sh";
fs.writeFileSync(fileName, deployScripts.join("\r\n"));

const fs = require('fs');

/**
 * 生成配置文件
 */
// let workers = [
//     { "name": "k8s-master", "ip": "10.180.98.130", "cpucores": 8 },
//     { "name": "k8s-node01", "ip": "10.180.98.131", "cpucores": 24 },
//     { "name": "k8s-node02", "ip": "10.180.98.132", "cpucores": 24 },
//     { "name": "k8s-node03", "ip": "10.180.98.133", "cpucores": 24 },
//     { "name": "k8s-node04", "ip": "10.180.98.134", "cpucores": 24 },
//     { "name": "k8s-node05", "ip": "10.180.98.135", "cpucores": 24 },
// ];

let workers = [{ "name": "localhost", "ip": "127.0.0.1", "cpucores": 3 }];

let fileName = "/home/kevin/git/lwdee/test/node_conf.json";

let json = fs.readFileSync(fileName);
let db = JSON.parse(json);
let port = db.port > 200 ? 100 : db.port + 2;

let index = 0;
let routerInfos = [];
for (var ni in workers) {
    let worker = workers[ni];
    for (var i = 0; i < worker.cpucores; i++) {
        index++;

        let portTail = (i + 1).toString().padStart(2, "0");
        routerInfos.push({ "nid": index, "ip": worker.ip, "dport": parseInt(port + portTail), "mport": parseInt((port + 1) + portTail) });
    }
}

let conf = { "node_amount": index, "port": port };
for (var ri in routerInfos) {
    let routerInfo = routerInfos[ri];
    conf["node" + routerInfo.nid] = {
        "nodeId": routerInfo.nid,
        "ip": routerInfo.ip,
        "dataPort": routerInfo.dport,
        "msgPort": routerInfo.mport,
        "routeInfo": routerInfos
    };
}

json = JSON.stringify(conf, null, "  ");

fs.writeFileSync(fileName, json);
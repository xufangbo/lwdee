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


let index = 0;
let routerInfos = [];
for (var ni in workers) {
    let worker = workers[ni];
    for (var i = 0; i < worker.cpucores; i++) {
        index++;

        let portTail = (i + 1).toString().padStart(2, "0");
        routerInfos.push({ "nid": index, "ip": worker.ip, "dport": parseInt("120" + portTail), "mport": parseInt("130" + portTail) });
    }
}

let conf = { "node_amount": index };
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

console.log(conf);
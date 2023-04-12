const fs = require('fs');

let fileName = "/home/kevin/git/lwdee/config/dsc.json";
let json = fs.readFileSync(fileName);
let dscConfig = JSON.parse(json);
let workers = dscConfig.workers;

let imgVersion = "v1.0.8";
fileName = "/home/kevin/git/lwdee/config/conf.json";

json = fs.readFileSync(fileName);
let db = JSON.parse(json);
// let port = db.port;
let port = db.port > 800 ? 100 : db.port + 10;
// if(dscConfig.name != "local"){
//     port = 100;
// }

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
let s1 = [];
let s2 = [];
let s3 = [];
let s4 = [];
let s5 = [];

let preWorker;
for (var i in routerInfos) {
    let router = routerInfos[i];
    if (router.worker != preWorker) {
        preWorker = router.worker;
    }
    s1.push(`ssh root@${router.worker} "docker stop dsc${router.nid}"`);
    s2.push(`ssh root@${router.worker} "docker rm dsc${router.nid}"`);
    s3.push(`ssh root@${router.worker} "docker start dsc${router.nid}"`);

    s4.push(`docker stop dsc${router.nid}`);
    s5.push(`docker rm dsc${router.nid}`);
}

// s1.forEach(x=> deployScripts.push(x));
// deployScripts.push("");
// deployScripts.push("");
// s2.forEach(x=> deployScripts.push(x));
// deployScripts.push("");
// deployScripts.push("");
// s3.forEach(x=> deployScripts.push(x));
// deployScripts.push("");
// deployScripts.push("");
// s4.forEach(x=> deployScripts.push(x));
// deployScripts.push("");
// deployScripts.push("");
// s5.forEach(x=> deployScripts.push(x));
// deployScripts.push("");
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

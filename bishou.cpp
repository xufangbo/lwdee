#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int len = 6;
std::string nodes[] = {"k8s-master", "k8s-node01", "k8s-node02", "k8s-node03", "k8s-node04", "k8s-node05"};
std::string containers[] = {"dsc1", "dsc2", "dsc3", "dsc4", "dsc5", "dsc6"};

void app_task(std::string nodeName) {
  string script = string("scp /home/kevin/git/lwdee/build/app root@") + nodeName + ":/home/kevin/git/lwdee/build/";
  system(script.c_str());
}

void config_task(std::string nodeName) {
  string script = string("scp /home/kevin/git/lwdee/config/dsc.json root@") + nodeName + ":/home/kevin/git/lwdee/config/";
  system(script.c_str());

  script = string("scp /home/kevin/git/lwdee/config/conf.json root@") + nodeName + ":/home/kevin/git/lwdee/config/";
  system(script.c_str());
}

void stop_task(std::string nodeName, string container) {
  string script = string("ssh root@") + nodeName + " \"docker stop " + container + "\"";
  system(script.c_str());
}

void start_task(std::string nodeName, string container) {
  string script = string("ssh root@") + nodeName + " \"docker start " + container + "\"";
  system(script.c_str());
}

void rm_task(std::string nodeName, string container) {
  string script = string("ssh root@") + nodeName + " \"docker rm " + container + "\"";
  system(script.c_str());
}

void log_task(std::string nodeName, string container) {
  string script = string("ssh root@") + nodeName + " \"rm -rf /home/kevin/git/lwdee/log/*\"";
  system(script.c_str());
}

void ps_task(std::string nodeName, string container) {
  string script = string("ssh root@") + nodeName + " \"docker ps \"";
  // system(script.c_str());

  script = string("ssh root@") + nodeName + " \"docker ps  -l\"";
  system(script.c_str());
}

int main(int argv, char** argc) {
  if (argv <= 1) {
    cout << "please input args" << endl;
    return 1;
  }

  string par = argc[1];

  thread ts[6];
  if (par == "app") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(app_task, nodes[i]);
    }
  } else if (par == "config") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(config_task, nodes[i]);
    }
  } else if (par == "stop") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(stop_task, nodes[i], containers[i]);
    }
  } else if (par == "start") {
    for (int i = 1; i < 6; i++) {
      ts[i] = thread(start_task, nodes[i], containers[i]);
    }
    sleep(0);
    ts[0] = thread(start_task, nodes[0], containers[0]);
  } else if (par == "rm") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(rm_task, nodes[i], containers[i]);
    }
  } else if (par == "log") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(log_task, nodes[i], containers[i]);
    }
  } else if (par == "ps") {
    for (int i = 0; i < 6; i++) {
      ts[i] = thread(ps_task, nodes[i], containers[i]);
    }
  } else {
    cout << "no " << par << endl;
  }

  for (int i = 0; i < 6; i++) {
    ts[i].join();
  }

  cout << "-----------------\nok" << endl;
}
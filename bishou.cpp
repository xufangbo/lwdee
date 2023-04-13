#include <memory.h>
#include <unistd.h>
#include <fstream>
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
  cout << script << endl;
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

void docker_run_task() {
  auto fileName = "./scripts/docker.sh";
  fstream f(fileName, ios_base::in);
  if (!f.is_open()) {
    cout << "can't open file " << fileName << endl;
  }

  int BUF_SIZE = 1024;
  char line[BUF_SIZE];
  while (!f.eof()) {
    memset(line, BUF_SIZE, '\0');
    f.getline(line, BUF_SIZE);
    if (strcmp(line, "----")) {
      sleep(2);
    }
    cout << line << endl;
    system(line);
  }
}

void js_task() {
  auto fileName = "./scripts/conf.js";
  string cmd = string("node ") + fileName;
  system(cmd.c_str());
}

int main(int argv, char** argc) {
  if (argv <= 1) {
    cout << "please input args" << endl;
    return 1;
  }

  string par = argc[1];

  thread ts[len];
  bool is_multi_thread = true;
  if (par == "app") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(app_task, nodes[i]);
    }
  } else if (par == "config") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(config_task, nodes[i]);
    }
  } else if (par == "stop") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(stop_task, nodes[i], containers[i]);
    }
  } else if (par == "start") {
    for (int i = len; i > 0; i--) {
      if (i == 1) {
        sleep(1);
      }
      ts[i-1] = thread(start_task, nodes[i-1], containers[i-1]);
    }
    sleep(0);
    ts[0] = thread(start_task, nodes[0], containers[0]);
  } else if (par == "rm") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(rm_task, nodes[i], containers[i]);
    }
  } else if (par == "log") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(log_task, nodes[i], containers[i]);
    }
  } else if (par == "ps") {
    for (int i = 0; i < len; i++) {
      ts[i] = thread(ps_task, nodes[i], containers[i]);
    }
  } else if (par == "docker") {
    is_multi_thread = false;
    docker_run_task();
  } else if (par == "js") {
    is_multi_thread = false;
    js_task();
  } else {
    cout << "no " << par << endl;
  }

  if (is_multi_thread) {
    for (int i = 0; i < len; i++) {
      ts[i].join();
    }
  }

  cout << "-----------------\nok" << endl;
}
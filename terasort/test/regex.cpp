#include <iostream>
#include <regex>
#include <sstream>

using namespace std;

int main() {
  string input = "? 309 ? a station full of ordinary people. Behind him stood Aunt Petunia and Dudley, looking terrified at the very sight of Harry. “You must be Harry’s family!” said Mrs. Weasley. “In a manner of speaking,” said Uncle Vernon. “Hurry up, boy, we haven’t got all day.” He walked away. Harry hung back for a last word with Ron and Hermione. “See you over the summer, then.” “Hope you have — er — a good holiday,” said Hermione, look?ing uncertainly after Uncle Vernon, shocked that anyone could be so unpleasant. “Oh, I will,” said Harry, and they were surprised at the grin that was spreading over his face. “They don’t know we’re not allowed to use magic at home. I’m going to have a lot of fun with Dudley this summer. . . .” This book ";
  std::regex pattern1("[0-9]");
  auto output1 = std::regex_replace(input, pattern1, "");

  cout << "output1: " << output1 << endl
       << endl;

  std::regex pattern2("[,|-|.|?|:|-|”|“|!|;|’s]");
  auto output2 = std::regex_replace(output1, pattern2, " ");

  cout << "output2: " << output2 << endl
       << endl;

  stringstream iss(output2);  // 输入流
  string token;               // 接收缓冲区

  while (!iss.eof()) {
    iss >> token;
    if (!token.empty()) {
      cout << token << " ";  // 输出
    }
  }

  return 0;
}

#include "suspend.hpp"

await<int> invokeService() {
  auto wait = await<int>([](suspend::SuspendHandler handle, int *p) {
    // *p = 2025;
    // handle.resume();
    int *ptr = (int*)p;
    std::thread([handle, ptr]() mutable {
      *ptr = 2023;
      std::cout << "sleep 2 seconds \n";
      std::this_thread::sleep_for(std::chrono::seconds(2));
      handle.resume();  // 可以简写为 handle();
    }).detach();
  });
  return wait;
}

suspend test() {
  auto rc = co_await invokeService();
  std::cout << "rc = " << rc << std::endl;
  std::cout << "3 thread id=" << std::this_thread::get_id() << "\n";
}

// int main() {
//   std::cout << "1 thread id=" << std::this_thread::get_id() << "\n";

//   test();

//   std::cout << "2 thread id=" << std::this_thread::get_id() << "\n";
//   std::this_thread::sleep_for(std::chrono::seconds(4));

//   return 0;
// }
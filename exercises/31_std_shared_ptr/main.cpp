#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10); // 引用计数为 1
    std::shared_ptr<int> ptrs[]{shared, shared, shared}; // 引用计数为 4

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 4, ""); // weak_ptr 不影响引用计数

    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, ""); 

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, "");

    ptrs[2] = std::make_shared<int>(*shared); // 创建了新的 shared_ptr
    // std::cout << "now " << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 1, "");

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    ASSERT(observer.use_count() == 3, "");

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    ASSERT(observer.use_count() == 2, "");

    shared = observer.lock();
    //lock() 返回一个 std::shared_ptr：
    //如果 *this 是非空的，则返回一个指向与 *this 共享对象的 std::shared_ptr 对象。
    //否则返回一个空的 std::shared_ptr 对象。
    ASSERT(observer.use_count() == 3, "");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "");

    return 0;
}

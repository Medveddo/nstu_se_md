#include <iostream>

int main(int argc, char const *argv[])
{
    long a = -7000;
    long b = 200000;
    std::cout << static_cast<float>(a) / static_cast<float>(b) << std::endl;
    return 0;
}

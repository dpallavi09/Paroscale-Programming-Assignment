#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
using namespace std;

mutex mtx;
vector<int> sharedList;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(1, 100);

void producer()
{
    int number = dis(gen);

    lock_guard<mutex> lock(mtx);

    sharedList.push_back(number);
    cout << "Producer generated: " << number << endl;
}

void consumer()
{
    lock_guard<mutex> lock(mtx);

    if (!sharedList.empty())
    {
        int number = sharedList.front();
        sharedList.erase(sharedList.begin());
        cout << "Consumer dequeued: " << number << endl;
    }
}

int main()
{
    thread producerThread1(producer);
    thread producerThread2(producer);

    thread consumerThread1(consumer);
    thread consumerThread2(consumer);

    producerThread1.join();
    producerThread2.join();
    consumerThread1.join();
    consumerThread2.join();

    return 0;
}

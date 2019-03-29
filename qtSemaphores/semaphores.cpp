// TDDO Read from buffer in chunks.
#include <QtCore>
#include <iostream>

const int DataSize = 100000;
const int BufferSize = 8192;
char buffer[BufferSize];

QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes(0);

class Producer : public QThread
{
 public:
     void run() override;
}; // class Producer

void Producer::run()
{
    for(int i = 0;i < DataSize;++i)
    {
       freeBytes.acquire();
       buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
       usedBytes.release();
    }
}

class Consumer : public QThread
{
 public:
     void run() override;
}; // class Consumer

void Consumer::run()
{
    for(int i=0;i < DataSize;++i)
    {
        usedBytes.acquire();
        std::cout << buffer[i % BufferSize];
        freeBytes.release();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return 0;
}

#include <iostream>
#include <vector>

class CircularQueue {
private:
    std::vector<int> data;     
    int front;                
    int rear;                  
    int count;                 
    int capacity;              

public:
    CircularQueue(int size) : capacity(size), front(0), rear(0), count(0) {
        data.resize(size, -1);  //  vector with -1 to indicate empty slots
    }


    void enqueue(int value) {
        if (count == capacity) {

            front = (front + 1) % capacity;
        }
        else {
            count++;
        }
        data[rear] = value;         
        rear = (rear + 1) % capacity; 
    }


    void dequeue() {
        if (count == 0) {
            std::cout << "Queue is empty. No data to dequeue." << std::endl;
            return;
        }
        data[front] = -1;           
        front = (front + 1) % capacity; 
        count--;
    }

    void display() {
        if (count == 0) {
            std::cout << "Queue is empty. Nothing to display." << std::endl;
            return;
        }

        std::cout << "Data in the queue (oldest to newest):" << std::endl;
        for (int i = 0; i < count; i++) {
            int index = (front + i) % capacity;
            std::cout << data[index] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    CircularQueue cq(7);  
   
    cq.enqueue(1);  
    cq.enqueue(2);  
    cq.enqueue(3);  
    cq.enqueue(4);  

    cq.display();    

    cq.enqueue(5);  
    cq.enqueue(6);  
    cq.enqueue(7);  
    cq.display();     

    cq.enqueue(8);  
    cq.display();     

    cq.dequeue();     
    cq.display();    


    cq.dequeue();
    cq.display();

    return 0;
}
/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef PACKET_H
#define PACKET_H

class Packet {

    private:
    
    public:
    
    std::vector<double> genome;
    std::pair<int,int> senderId;
    float sigma = 0.0;
    float fitness = 0.0;
    
    Packet()
    {
    }
    
    void display()
    {
        std::cout << "[PACKET]\n";
        std::cout << "\tsender's id  = " << std::get<0>(senderId) << "::" << std::get<1>(senderId) << "\n";
        std::cout << "\tfitnessValue = " << fitness << "\n";
        std::cout << "\tsigma        = " << sigma << "\n";
    }

};

#endif

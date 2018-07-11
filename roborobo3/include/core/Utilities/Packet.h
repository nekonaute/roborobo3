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
    std::pair<int,int> agentId;
    float sigma = 0.0;
    float fitness = 0.0;
    
    Packet()
    {
        fitness = 14;
    }
    
    void display()
    {
        std::cout << "[PACKET]\n";
        std::cout << "\tsender's id  = " << std::get<0>(agentId) << "::" << std::get<1>(agentId) << "\n";
        std::cout << "\tfitnessValue = " << fitness << "\n";
        std::cout << "\tsigma        = " << sigma << "\n";
    }

};

#endif

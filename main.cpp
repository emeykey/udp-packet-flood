#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

#include <arpa/inet.h> // htons, inet_addr
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h> // uint16_t
#include <sys/socket.h> // socket, sendto
#include <unistd.h> // close

float bandwidth_limit;

int main(int argc, char *argv[])
{
    if (argc >= 4)
    {
        auto start_time = std::chrono::high_resolution_clock::now(); //Cas spusteni programu
        float traffic_count = 0; //Celkova velikost odeslanych dat
        float bandwidth_count = 0; //To same ale pro vypocet rychlosti
        std::string msg = "";
        float bandwidth_limit = 0;
        for (int i=0; i < atoi(argv[3]); i++) //Vytvori obsah paketu po uroven definovane velikosti
            msg = msg + "A";

        while (true) //Loop
        {
            std::string hostname{argv[1]}; //IP podle argumentu
            uint16_t port = atoi(argv[2]); //Port podle argumentu

            int sock = ::socket(AF_INET, SOCK_DGRAM, 0);

            sockaddr_in destination;
            destination.sin_family = AF_INET;
            destination.sin_port = htons(port);
            destination.sin_addr.s_addr = inet_addr(hostname.c_str());

            int n_bytes = ::sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination)); //Posle paket
            auto current_time = std::chrono::high_resolution_clock::now(); //Aktualni cas
            traffic_count = traffic_count + (float)n_bytes/1024/1024; //Pripocte k celkovemu poctu velikost aktualne odeslaneho paketu
            bandwidth_count = bandwidth_count + (float)n_bytes*8/1024/1024; //To same akorat kvuli pocitani rychlosti
            float bandwidth = bandwidth_count/(float)std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count(); //Rychlost v megabitech za vterinu
            std::cout << std::fixed << std::setprecision(2) << "Bandwidth used: " << bandwidth << " Mb/s | " << traffic_count << " MB total traffic sent." << "\r"; //Vyplivne aktualni info
            ::close(sock);
            if (argv[4] != NULL)
            {
                bandwidth_limit = atof(argv[4]);
                if (bandwidth_limit != 0 && bandwidth > bandwidth_limit)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }

        return 0;
    }

    else
    {
        std::cout << "Usage: ./packetspam <ip> <port> <bytes> [speed_in_mbps_default_max]" << std::endl << "Example: ./packetspam 192.168.0.1 80 4096 100" << std::endl;
        return 1;
    }
}

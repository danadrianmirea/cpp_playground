#pragma once

#include <iostream>

/**
 * Round Robin Load Balancing Algorithm Implementation
 *
 * This example demonstrates three types of round robin algorithms:
 * 1. Basic Round Robin - Equal distribution across all servers
 * 2. Weighted Round Robin - Distribution based on server capacity
 * 3. Smooth Weighted Round Robin - Prevents burst allocation to high-capacity servers
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

struct Server {
    std::string id;
    std::string address;
    int weight;      // For weighted round robin
    int currentWeight; // Internal counter for smooth weighted RR

    Server(std::string i, std::string addr, int w = 1)
        : id(std::move(i)), address(std::move(addr)), weight(w), currentWeight(w) {}
};

class BasicRoundRobin {
private:
    std::vector<Server> servers;
    size_t currentIndex;

public:
    BasicRoundRobin() : currentIndex(0) {}

    void addServer(const std::string& id, const std::string& address) {
        servers.emplace_back(id, address);
    }

    void removeServer(const std::string& id) {
        servers.erase(
            std::remove_if(servers.begin(), servers.end(),
                [&id](const Server& s) { return s.id == id; }),
            servers.end()
        );

        // Adjust current index if needed
        if (currentIndex >= servers.size()) {
            currentIndex = 0;
        }
    }

    Server& selectServer() {
        if (servers.empty()) {
            throw std::runtime_error("No servers available");
        }

        Server& selected = servers[currentIndex];
        currentIndex = (currentIndex + 1) % servers.size();
        return selected;
    }

    void printServers() const {
        std::cout << "\nRegistered Servers:\n";
        std::cout << std::string(50, '-') << "\n";
        for (const auto& server : servers) {
            std::cout << "  ID: " << server.id
                      << " | Address: " << server.address << "\n";
        }
        std::cout << "\n";
    }

    size_t serverCount() const { return servers.size(); }
};

class WeightedRoundRobin {
private:
    std::vector<Server> servers;

public:
    WeightedRoundRobin() {}

    void addServer(const std::string& id, const std::string& address, int weight) {
        if (weight <= 0) {
            throw std::invalid_argument("Weight must be positive");
        }
        servers.emplace_back(id, address, weight);
    }

    void removeServer(const std::string& id) {
        servers.erase(
            std::remove_if(servers.begin(), servers.end(),
                [&id](const Server& s) { return s.id == id; }),
            servers.end()
        );
    }

    Server& selectServer() {
        if (servers.empty()) {
            throw std::runtime_error("No servers available");
        }

        // Calculate total weight
        int totalWeight = 0;
        for (const auto& server : servers) {
            totalWeight += server.weight;
        }

        // Find server with highest currentWeight
        Server* selected = &servers[0];
        for (auto& server : servers) {
            server.currentWeight += server.weight;
            if (server.currentWeight > selected->currentWeight) {
                selected = &server;
            }
        }

        // Subtract total weight from selected server's currentWeight
        selected->currentWeight -= totalWeight;

        return *selected;
    }

    void printServers() const {
        std::cout << "\nRegistered Servers (Weighted):\n";
        std::cout << std::string(50, '-') << "\n";
        for (const auto& server : servers) {
            std::cout << "  ID: " << server.id
                      << " | Address: " << server.address
                      << " | Weight: " << server.weight << "\n";
        }
        std::cout << "\n";
    }

    size_t serverCount() const { return servers.size(); }
};

class SmoothWeightedRoundRobin {
private:
    std::vector<Server> servers;

public:
    void addServer(const std::string& id, const std::string& address, int weight) {
        if (weight <= 0) {
            throw std::invalid_argument("Weight must be positive");
        }
        servers.emplace_back(id, address, weight);
    }

    void removeServer(const std::string& id) {
        servers.erase(
            std::remove_if(servers.begin(), servers.end(),
                [&id](const Server& s) { return s.id == id; }),
            servers.end()
        );
    }

    Server& selectServer() {
        if (servers.empty()) {
            throw std::runtime_error("No servers available");
        }

        // Calculate total weight
        int totalWeight = 0;
        for (const auto& server : servers) {
            totalWeight += server.weight;
        }

        // Find server with highest currentWeight
        Server* selected = &servers[0];
        int maxCurrentWeight = selected->currentWeight;

        for (auto& server : servers) {
            // Add original weight to currentWeight
            server.currentWeight += server.weight;
            if (server.currentWeight > maxCurrentWeight) {
                maxCurrentWeight = server.currentWeight;
                selected = &server;
            }
        }

        // Subtract total weight from selected server
        selected->currentWeight -= totalWeight;

        return *selected;
    }

    void printServers() const {
        std::cout << "\nRegistered Servers (Smooth Weighted):\n";
        std::cout << std::string(50, '-') << "\n";
        for (const auto& server : servers) {
            std::cout << "  ID: " << server.id
                      << " | Address: " << server.address
                      << " | Weight: " << server.weight
                      << " | CurrentWeight: " << server.currentWeight << "\n";
        }
        std::cout << "\n";
    }

    size_t serverCount() const { return servers.size(); }
};

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void printDistribution(const std::string& algorithm,
                       const std::vector<std::string>& selections,
                       int requestCount) {
    std::cout << "\n" << algorithm << " - Request Distribution:\n";
    std::cout << std::string(40, '-') << "\n";

    // Count selections
    std::vector<std::pair<std::string, int>> counts;
    for (const auto& selection : selections) {
        bool found = false;
        for (auto& count : counts) {
            if (count.first == selection) {
                count.second++;
                found = true;
                break;
            }
        }
        if (!found) {
            counts.emplace_back(selection, 1);
        }
    }

    // Print distribution
    for (const auto& [serverId, count] : counts) {
        double percentage = (static_cast<double>(count) / requestCount) * 100;
        std::cout << "  " << serverId << ": " << count << " requests ("
                  << percentage << "%)\n";
    }

    // Verify distribution
    int total = 0;
    for (const auto& count : counts) {
        total += count.second;
    }
    std::cout << "\n  Total: " << total << " requests distributed\n";
}

void demoBasicRoundRobin() {
    printHeader("Basic Round Robin Load Balancer");

    BasicRoundRobin lb;

    // Add servers
    lb.addServer("server-1", "192.168.1.10:8080");
    lb.addServer("server-2", "192.168.1.11:8080");
    lb.addServer("server-3", "192.168.1.12:8080");

    lb.printServers();

    // Simulate request distribution
    int requestCount = 15;
    std::vector<std::string> selections;

    std::cout << "Simulating " << requestCount << " requests...\n";
    std::cout << std::string(40, '-') << "\n";

    for (int i = 1; i <= requestCount; ++i) {
        Server& server = lb.selectServer();
        selections.push_back(server.id);
        std::cout << "Request " << i << " -> " << server.id
                  << " (" << server.address << ")\n";
    }

    printDistribution("Basic Round Robin", selections, requestCount);
}

void demoWeightedRoundRobin() {
    printHeader("Weighted Round Robin Load Balancer");

    WeightedRoundRobin lb;

    // Add servers with different weights
    // server-1: weight 5 (high capacity)
    // server-2: weight 2 (medium capacity)
    // server-3: weight 1 (low capacity)
    lb.addServer("server-1", "192.168.1.10:8080", 5);
    lb.addServer("server-2", "192.168.1.11:8080", 2);
    lb.addServer("server-3", "192.168.1.12:8080", 1);

    lb.printServers();

    // Simulate request distribution
    int requestCount = 24; // Multiple of 8 (5+2+1)
    std::vector<std::string> selections;

    std::cout << "Simulating " << requestCount << " requests...\n";
    std::cout << std::string(40, '-') << "\n";

    for (int i = 1; i <= requestCount; ++i) {
        Server& server = lb.selectServer();
        selections.push_back(server.id);
        std::cout << "Request " << i << " -> " << server.id
                  << " (weight: " << server.weight << ")\n";
    }

    printDistribution("Weighted Round Robin", selections, requestCount);

    // Show expected ratio explanation
    std::cout << "\nExpected distribution (weight ratio 5:2:1):\n";
    std::cout << "  server-1: ~62.5% (5/8)\n";
    std::cout << "  server-2: ~25.0% (2/8)\n";
    std::cout << "  server-3: ~12.5% (1/8)\n";
}

void demoSmoothWeightedRoundRobin() {
    printHeader("Smooth Weighted Round Robin (Nginx Algorithm)");

    SmoothWeightedRoundRobin lb;

    // Add servers with same weights as before
    lb.addServer("server-1", "192.168.1.10:8080", 5);
    lb.addServer("server-2", "192.168.1.11:8080", 2);
    lb.addServer("server-3", "192.168.1.12:8080", 1);

    lb.printServers();

    // Simulate request distribution
    int requestCount = 24;
    std::vector<std::string> selections;

    std::cout << "Simulating " << requestCount << " requests...\n";
    std::cout << std::string(40, '-') << "\n";

    for (int i = 1; i <= requestCount; ++i) {
        Server& server = lb.selectServer();
        selections.push_back(server.id);
        std::cout << "Request " << i << " -> " << server.id
                  << " (weight: " << server.weight << ")\n";
    }

    printDistribution("Smooth Weighted Round Robin", selections, requestCount);
}

void demoDynamicServerManagement() {
    printHeader("Dynamic Server Management");

    BasicRoundRobin lb;

    // Simulate adding/removing servers during operation
    std::cout << "Initial state: Adding 3 servers\n";
    lb.addServer("server-1", "192.168.1.10:8080");
    lb.addServer("server-2", "192.168.1.11:8080");
    lb.addServer("server-3", "192.168.1.12:8080");

    std::cout << "\nFirst 6 requests:\n";
    for (int i = 1; i <= 6; ++i) {
        Server& server = lb.selectServer();
        std::cout << "  Request " << i << " -> " << server.id << "\n";
    }

    std::cout << "\nRemoving server-2...\n";
    lb.removeServer("server-2");
    lb.printServers();

    std::cout << "Next 4 requests:\n";
    for (int i = 1; i <= 4; ++i) {
        Server& server = lb.selectServer();
        std::cout << "  Request " << i << " -> " << server.id << "\n";
    }

    std::cout << "\nAdding server-4...\n";
    lb.addServer("server-4", "192.168.1.13:8080");
    lb.printServers();
}

int testRoundRobin() {

    // Run all demos
    demoBasicRoundRobin();
    demoWeightedRoundRobin();
    demoSmoothWeightedRoundRobin();
    demoDynamicServerManagement();
    return 0;
}
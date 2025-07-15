#include <iostream>
#include <vector>

class Room {
protected:
    int points;
    Room* rightNeighbor;
    Room* lowerNeighbor;
public:
    virtual int get_points() = 0;

    Room* get_rightNeighbor() {
        return rightNeighbor;
    }

    Room* get_lowerNeighbor() {
        return lowerNeighbor;
    }

    Room(int p, Room* rn, Room* ln) {
        points = p;
        rightNeighbor = rn;
        lowerNeighbor = ln;
    }

    virtual ~Room() {}
};


class Knight {
    int min_hp;

public:
    Knight() {
        min_hp = 1;
    }

    void visit(Room* room) {
        min_hp += room->get_points();
    }
};


class ThreatRoom : public Room {
public:
    int get_points() override {
        return points * -1;
    }

    ThreatRoom(int p, Room* rn, Room* ln) : Room(p, rn, ln) {}
};


class PowerUpRoom : public Room {
    public:
    int get_points() override {
        return points;
    }

    PowerUpRoom(int p, Room* rn, Room* ln) : Room(p, rn, ln) {}
};

class Dungeon {
    std::vector<Room*> rooms;

    Room* createRoom(int roomPoints, int rightNeighborPoints, int leftNeighborPoints) {
        if (roomPoints < 0) {
            ThreatRoom currentRoom(roomPoints, nullptr, nullptr);
        }
    }
public:
    Dungeon(std::vector<std::vector<int>> dungeon) {
        for (size_t i = 0; i < dungeon.size(); i++) {
            for (size_t j = 0; j < dungeon[i].size(); j++) {

            }
        }
    }
};


int main() {

    return 0;
}
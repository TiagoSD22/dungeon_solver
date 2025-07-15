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

    void addRightNeighbor(Room* rn) {
        rightNeighbor = rn;
    }

    void addLowerNeighbor(Room* ln) {
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

class NeutralRoom : public Room {
    public:
    int get_points() override {
        return 0;
    }

    NeutralRoom(Room* rn, Room* ln) : Room(0, rn, ln) {}
};

class Dungeon {
    size_t rows;
    size_t cols;
    std::vector<std::vector<Room*>> rooms;

    static Room* createRoom(int roomPoints) {
        if (roomPoints < 0) {
            return new ThreatRoom(roomPoints, nullptr, nullptr);
        }

        if (roomPoints == 0) {
            return new NeutralRoom(nullptr, nullptr);
        }

        return new PowerUpRoom(roomPoints, nullptr, nullptr);

    }
public:
    Dungeon(std::vector<std::vector<int>> dungeon) {
        rows = dungeon.size();
        cols = dungeon[0].size();

        rooms.resize(rows);

        for (size_t i = 0; i < rows; i++) {
            rooms[i].resize(cols, nullptr);
            for (size_t j = 0; j < cols; j++) {
                Room* currentRoom = rooms[i][j];
                Room* rightNeighborRoom = nullptr;
                Room* lowerNeighborRoom = nullptr;

                if (nullptr == currentRoom) {
                    currentRoom = createRoom(dungeon[i][j]);
                }

                if (j < cols - 1) {
                    rightNeighborRoom = rooms[i][j + 1];
                    if (nullptr == rightNeighborRoom) {
                        rightNeighborRoom = createRoom(dungeon[i][j + 1]);
                    }
                }

                if (i < rows - 1) {
                    lowerNeighborRoom = rooms[i + 1][j];
                    if (nullptr == lowerNeighborRoom) {
                        lowerNeighborRoom = createRoom(dungeon[i + 1][j]);
                    }
                }
                currentRoom->addRightNeighbor(rightNeighborRoom);
                currentRoom->addLowerNeighbor(lowerNeighborRoom);
                rooms[i][j] = currentRoom;
            }
        }
    }
};


int main() {
    Knight* knight = new Knight();
    Dungeon* dungeon = new Dungeon({{-2,-3,3},{-5,-10,1},{10,30,-5}});

    return 0;
}
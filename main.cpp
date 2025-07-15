#include <iostream>
#include <vector>

class Room {
protected:
    int points;
    Room* rightNeighbor;
    Room* lowerNeighbor;
public:
    virtual int getPoints() = 0;

    Room(int p, Room* rn, Room* ln) {
        points = std::abs(p);
        rightNeighbor = rn;
        lowerNeighbor = ln;
    }

    Room* getRightNeighbor() {
        return rightNeighbor;
    }

    Room* getLowerNeighbor() {
        return lowerNeighbor;
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
    int hp;

public:
    Knight() {
        hp = 0;
    }

    int getHp() {
        return hp;
    }

    void visit(Room* room) {
        hp += room->getPoints();
    }

    static Room* decideNextRoom(Room* currentRoom) {
        Room* rightNeighborRoom = currentRoom->getRightNeighbor();
        Room* lowerNeighborRoom = currentRoom->getLowerNeighbor();
        if(nullptr == rightNeighborRoom){
            return lowerNeighborRoom;
        }
        if(nullptr == lowerNeighborRoom){
            return rightNeighborRoom;
        }
        if(rightNeighborRoom->getPoints() > lowerNeighborRoom->getPoints()){
            return rightNeighborRoom;
        }

        return lowerNeighborRoom;
    }
};


class ThreatRoom : public Room {
public:
    int getPoints() override {
        return points * -1;
    }

    ThreatRoom(int p, Room* rn, Room* ln) : Room(p, rn, ln) {}
};


class PowerUpRoom : public Room {
    public:
    int getPoints() override {
        return points;
    }

    PowerUpRoom(int p, Room* rn, Room* ln) : Room(p, rn, ln) {}
};

class NeutralRoom : public Room {
    public:
    int getPoints() override {
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
        }

        for (size_t i = 0; i < rows; i++) {
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
                        rooms[i][j + 1] = rightNeighborRoom;
                    }
                }

                if (i < rows - 1) {
                    lowerNeighborRoom = rooms[i + 1][j];
                    if (nullptr == lowerNeighborRoom) {
                        lowerNeighborRoom = createRoom(dungeon[i + 1][j]);
                        rooms[i + 1][j] = lowerNeighborRoom;
                    }
                }
                currentRoom->addRightNeighbor(rightNeighborRoom);
                currentRoom->addLowerNeighbor(lowerNeighborRoom);
                rooms[i][j] = currentRoom;
            }
        }
    }

    Room* getInitialRoom() {
        return rooms[0][0];
    }

    Room* getPrincessRoom() {
        return rooms[rows - 1][cols - 1];
    }
};

class DungeonSolver {
    Dungeon* dungeon;
    Knight* knight;

public:
    DungeonSolver(Dungeon* dungeon, Knight* knight) {
        this->dungeon = dungeon;
        this->knight = knight;
    }

    int solve() {
        Room* initialRoom = dungeon->getInitialRoom();
        Room* princessRoom = dungeon->getPrincessRoom();
        Room* currentRoom = initialRoom;

        do {
            knight->visit(currentRoom);
            currentRoom = knight->decideNextRoom(currentRoom);
        } while (currentRoom != princessRoom && nullptr != currentRoom);

        if (nullptr != currentRoom) {
            knight->visit(currentRoom);
        }

        return std::abs(knight->getHp()) + 1;
    }
};


int main() {
    //std::vector<std::vector<int>> input = {{0}}; //{{-2,-3,3},{-5,-10,1},{10,30,-5}};
    std::vector<std::vector<int>> input = {{-2,-3,3},{-5,-10,1},{10,30,-5}};

    Knight* knight = new Knight();
    Dungeon* dungeon = new Dungeon(input);
    DungeonSolver dungeonSolver(dungeon, knight);

    int result = dungeonSolver.solve();
    std::cout << result << std::endl;

    return 0;
}
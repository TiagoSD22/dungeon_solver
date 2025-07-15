#include <iostream>
#include <vector>
#include <limits>

#define INF std::numeric_limits<int>::max()

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
    int totalDmgReceived;
    int totalPowerUpReceived;
    int min_hp;

public:
    Knight() {
        hp = 0;
        totalDmgReceived = 0;
        totalPowerUpReceived = 0;
        min_hp = 1;
    }

    int getHp() {
        return hp;
    }

    int getTotalDmgReceived() {
        return totalDmgReceived;
    }

    int getTotalPowerUpReceived() {
        return totalPowerUpReceived;
    }

    int getMinHp() {
        return min_hp;
    }

    void resetStatus(){
        hp = 0;
        totalDmgReceived = 0;
        totalPowerUpReceived = 0;
        min_hp = 1;
    }

    void visit(Room* room) {
        if(room->getPoints() < 0){
            totalDmgReceived += room->getPoints();
        }
        else if(room->getPoints() > 0){
            totalPowerUpReceived += room->getPoints();
        }

        hp += room->getPoints();
        if (hp <= 0){
            //min_hp = std::abs(hp) + 1;

            if (totalDmgReceived != 0) {
                if (totalPowerUpReceived < std::abs(totalDmgReceived)){
                    min_hp = std::abs(totalPowerUpReceived + totalDmgReceived) + 1; //dmg is negative, then + instead of -
                    hp = min_hp + totalDmgReceived + totalPowerUpReceived;
                    //return min_hp;
                }
            }
            else {
                min_hp = std::abs(totalDmgReceived) + 1;
            }
        }
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

    bool isSingleRoom(){
        return rows == 1 && cols == 1;
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

    int recursiveSolver(Room* currentRoom, Knight k){
        if(nullptr != currentRoom){
            Room* princessRoom = dungeon->getPrincessRoom();

            k.visit(currentRoom);
            if (currentRoom == princessRoom){
                *knight = k;
                return knight->getMinHp();
            }

            int hpMinByRight = recursiveSolver(currentRoom->getRightNeighbor(), k);
            int hpMinByLower = recursiveSolver(currentRoom->getLowerNeighbor(), k);

            return std::min(hpMinByRight, hpMinByLower);
        }
        else{
            return INF;
        }
    }

    int solve() {
        if (!dungeon->isSingleRoom()) {
            Room* initialRoom = dungeon->getInitialRoom();
            return recursiveSolver(initialRoom, *knight);
        }

        Room* initialRoom = dungeon->getInitialRoom();
        knight->visit(initialRoom);

        return knight->getMinHp();
    }
};


int main() {
    //std::vector<std::vector<int>> input = {{0}}; //expected 1
    std::vector<std::vector<int>> input = {{-2,-3,3},{-5,-10,1},{10,30,-5}}; //expected 7
    //std::vector<std::vector<int>> input = {{2,1},{1,-1}}; //expected 1
    //std::vector<std::vector<int>> input = {{-3,5}}; //expected 4
    //std::vector<std::vector<int>> input = {{1,-3,3},{0,-2,0},{-3,-3,-3}}; //expected 3
    //std::vector<std::vector<int>> input = {{0,-5},{0,0}}; //expected 1

    Knight* knight = new Knight();
    Dungeon* dungeon = new Dungeon(input);
    DungeonSolver dungeonSolver(dungeon, knight);

    int result = dungeonSolver.solve();
    std::cout << result << std::endl;

    return 0;
}
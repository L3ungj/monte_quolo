#pragma once

#include <array>
#include <bitset>
#include <iostream>
#include <ostream>
#include <queue>
#include <vector>
using namespace std;

enum class MoveType {
    PAWN,
    HORI,
    VERT,
};

class Move {
    // convention: (r=1,c=1) is bottom left from p1's perspective.
public:
    MoveType type;
    char r;
    char c;

    Move() = default;
    Move(MoveType type, char r, char c);

    Move(string moveStr);
    friend ostream &operator<<(ostream &os, const Move &move);
};

class GameState {
public:
    array<array<bool, 8>, 8> horiWalls;
    array<array<bool, 8>, 8> vertWalls;
    char p1r, p1c, p2r, p2c;
    char p1WallsLeft, p2WallsLeft;
    bool p1Turn = true;

    GameState();
    GameState(const GameState &other);

    bool operator==(const GameState &other) const;

    bool isHoriBlocked(int r, int c) const;
    bool isVertBlocked(int r, int c) const;
    bool canMoveUp(int r, int c) const;
    bool canMoveDown(int r, int c) const;
    bool canMoveLeft(int r, int c) const;
    bool canMoveRight(int r, int c) const;

    array<array<int, 9>, 9> getDistToGoal(bool isP1) const;
    int p1Dist() const;
    int p2Dist() const;

    void print() const;
    bool isTerminal() const;
    bool p1Won() const;
    bool p2Won() const;
    bool isLegal(Move move, bool connCheck) const;
    bool makeMove(Move move, bool legalCheck);
    vector<Move> getLegalMoves(bool connCheck) const;
    vector<Move> getLegalPawnMoves() const;
    vector<Move> getLegalWallMoves(bool connCheck) const;

    Move deduceMove(const GameState &other) const; // Deduce the move that transforms this state to the other state. Assumes the move is legal.
};
#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "constants.h"
#include "player.h"

#include <iostream>
#include <unordered_set>
#include <vector>

class Game {
        friend class Board;

    private:
        bool merchantHostile = false;
        std::ostream& output;
        std::istream& layoutInput; // Input for layout, endl represents end of layout
        Board* currentBoard = nullptr;
        Player* player;
        /** If enemies/potions/gold should be randomly generated */
        bool shouldGenerate;
        std::vector<std::unordered_set<Tile*>> chambers = {};

        // update the board, then render the bottom bar. happens after a player action
        void update();
        void nextLevel();

        /**
         * @brief randomly populates floorTiles according to spec
         *
         * SIDE EFFECTS: shuffles `floorTiles`, modifies tile pointers
         *
         * @returns player location, stair location
         */
        static std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> randomPopulateMap(
            std::vector<std::unordered_set<Tile*>>& chambers, Board* newBoard, Player* player
        );

        /**
         * @brief go through each floor tile and try to find and label all chambers
         *
         * SIDE EFFECTS: modifies various `Tile*` pointers
         *
         * Total Θ(n) time because each floor tile is visited at most twice
         *
         * @returns vector of set of tiles which are floor tiles, with the same chamber
         * ID
         */
        static std::vector<std::unordered_set<Tile*>> labelChambers(
            std::vector<std::pair<size_t, size_t>>& floorTiles, const Board* const newBoard
        );

        /**
         * @brief DFS-style function to crawl the chamber and label every tile in the chamber
         *
         * SIDE EFFECTS: modified `recursiveChambers` and returns it
         *
         * @param currentChamberId - non-zero non-negative integer
         *
         * @returns set of tiles with the same chamber ID
         */
        static std::unordered_set<Tile*>& traverseChamber(
            std::pair<size_t, size_t>&& tile,
            const Board* const newBoard,
            size_t currentChamberId,
            std::unordered_set<Tile*>& recursiveChambers
        );

    public:
        Game(Player*, std::ostream&, std::istream&, bool generate);
        ~Game();

        /** @returns boolean indicating if player is still alive or not */
        bool playerMove(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerAttack(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerPickup(CardinalDirection);
        bool isAlive();

        // Render with no side effects to the output stream
        void render() const;
};

#endif

#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "constants.h"
#include "player.h"

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

struct Tile;

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

        // current level count "first is 1"
        int level = 0;

        int suitLevel; // randomly generate 0-4 on construction

        // update the board, then render the bottom bar. happens after a player action
        void update();
        // void nextLevel();

        /**
         * @brief randomly populates floorTiles according to spec
         *
         * SIDE EFFECTS: shuffles each set inside the vector `chambers`
         *
         * @returns player location, stair location
         */
        static void randomPopulateMap(Board* newBoard, Player* player, bool shouldGenerateSuit);

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
        static std::vector<std::vector<Tile*>>
            labelChambers(std::vector<Tile*>& floorTiles, const Board* const newBoard);

        /**
         * @brief DFS-style function to crawl the chamber and label every tile in the chamber
         *
         * SIDE EFFECTS: modified `recursiveChambers` and returns it
         *
         * @param currentChamberId - non-zero non-negative integer
         *
         * @returns set of tiles with the same chamber ID
         */
        static std::vector<Tile*>& traverseChamber(
            Tile* tile,
            const Board* const newBoard,
            std::size_t currentChamberId,
            std::vector<Tile*>& recursiveChambers
        );

    public:
        // temp
        void nextLevel();

        Game(Player*, std::ostream&, std::istream&, bool generate);
        ~Game();

        /** @returns boolean indicating if player is still alive or not */
        bool playerMove(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerAttack(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerPickup(CardinalDirection);

        // Render with no side effects to the output stream
        void render() const;

        bool didWin();

        int getScore() const;
};

#endif

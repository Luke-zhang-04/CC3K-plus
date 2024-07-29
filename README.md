# CCK3+

## Summary of Requirements

-   _Character_ is any person/animal/thing
    -   NPCs are enemies
-   Player can pick up/use _items_
-   Character has _HP_, _Atk_, and _Def_
-   _Cell_: wall, floor tile, doorway, passage
-   _1 block radius_: 1 tile around character (3x3 square)

Note: The first level is the top level, and the next level always refers to going down a level. The bottom level is the last level.

### Common Spawning Rule

Suppose we wish to spawn an item $x$:

-   Each chamber is equally likely to spawn $x$, regardless of size
-   Each floor tile of that chamber is equally likely to spawn $x$, if that chamber was chosen

In other words, if one chamber is huge and the other small, and we spawn many $x$'s, we expect to see the same number of $x$ in each chamber, with a higher density in the smaller chamber.

### Player Character

-   Denoted by `@`

**Races**:

| Race  | HP    | Atk  | Def  | Feature                               |
| ----- | ----- | ---- | ---- | ------------------------------------- |
| Human | `140` | `20` | `20` | $50\%$ increased score                |
| Dwarf | `100` | `20` | `30` | $2\times$ gold                        |
| Elf   | `140` | `30` | `10` | Negative potions have positive effect |
| Orc   | `180` | `30` | `25` | $\displaystyle \frac{1}{2}$ gold      |

**Question.** How could your design your system so that each race could be easily generated? Additionally, how difficult does such a solution make adding additional classes?

### Enemies

-   Randomly move within chamber (except dragons)
    -   Dragons always guard a treasure hoard (spawn within 1-block radius of pile of treasure, treasure hoard spawned => dragon spawned)
-   Enemies can't leave room they were spawned in
-   Enemies always attacks the player character if they are within a 1 block radius
-   Enemies randomly move 1 tile at a time assuming the tile is unoccupied, if there is no player within a 1 block radius
    -   Move enemies in line by line fashion (move top-left-most enemy first, and bottom-right-most enemy last)
    -   Only move once per player action (do not process a move for the same enemy twice)

| Class    | HP    | Atk  | Def  | Representation | Spawn chance                 | Notes                                                                                               |
| -------- | ----- | ---- | ---- | -------------- | ---------------------------- | --------------------------------------------------------------------------------------------------- |
| Vampire  | `50`  | `25` | `25` | `V`            | $\displaystyle \frac{3}{18}$ |                                                                                                     |
| Werewolf | `120` | `30` | `5`  | `W`            | $\displaystyle \frac{4}{18}$ |                                                                                                     |
| Troll    | `120` | `25` | `15` | `V`            | $\displaystyle \frac{2}{18}$ |                                                                                                     |
| Goblin   | `70`  | `5`  | `10` | `N`            | $\displaystyle \frac{5}{18}$ |                                                                                                     |
| Merchant | `30`  | `70` | `5`  | `M`            | $\displaystyle \frac{2}{18}$ | Attacking a merchant causes every merchant to be hostile                                            |
| Dragon   | `150` | `20` | `20` | `D`            | See treasure spawn rates     | Always guards a treasure hoard, only hostile when a player is in 1-block radius of its dragon hoard |
| Phoenix  | `50`  | `35` | `20` | `X`            | $\displaystyle \frac{2}{18}$ |                                                                                                     |

-   Spawning
    -   20 enemies are always spawned per floor
    -   Spawning follows the _common spawning rule_

**Question.** How does your system handle generating different enemies? Is it different from how you generate the player character? Why or why not?

**Question.** How could you implement special abilities for different enemies. For example, gold stealing for goblins, health regeneration for trolls, health stealing for vampires, etc.?

#### Combat

-   Hostile enemies auto-attack players in 1-block radius
    -   50% chance their attack misses
-   Player has option of attacking the 1 block radius around them
    -   Never misses
    -   PC has initiative

Damage calculation:

$$
\operatorname{damage}(\text{Defender}) = \left\lceil \frac{100}{100 + \operatorname{Def}(\text{Defender})} \cdot \operatorname{Atk}(\text{Attacker}) \right\rceil
$$

If player has a Barrier Suit, the damage calculation becomes:

$$
\operatorname{Damage}(PC) = \left\lceil \frac{\operatorname{Damage}(\text{Defender})}{2} \right\rceil
$$

i.e half damage.

### Items

-   Denoted with `p`
-   Two types: _positive_ and _negative_
    -   Effect of any potion is hidden from player until use
-   Potion can only be used when player is within 1-block radius

**Types of Potions**

| Potion                | Effect                                  |
| --------------------- | --------------------------------------- |
| `Restore health (RH)` | Restore up to 10 HP (cannot exceed max) |
| `Poision health (PH)` | Lose up to 10 HP                        |
| `Boost Atk (BA)`      | Increase Atk by 5                       |
| `Wound Atk (WA)`      | Decrease Atk by 5                       |
| `Boost Def (BD)`      | Increate Def by 5                       |
| `Wound Def (WD)`      | Decrease Def by 5                       |

-   Health potions are permanent
-   Boost and wound potions stay in effect for the current floor
-   `Atk` and `Def` cannot drop below 0
-   Spawning
    -   All potions are equally likely to spawn ($\frac{1}{6}$)
    -   10 potions are spawned per floor
    -   Spawning follows the _common spawning rule_

**Question.** What design pattern could you use to model the effects of temporary potions (Wound/Boost Atk/Def) so that you do not need to explicitly track which potions the player character has consumed on any particular floor?

### Treasure

-   Denoted by `G`
-   Consists of gold (so gold is synonymous with treasure)
-   Collected when walked over by player

**Classes of Treasure**

| Name           | Value | Spawn rate                  | Notes                                                                  |
| -------------- | ----- | --------------------------- | ---------------------------------------------------------------------- |
| Normal         | `1`   | $\displaystyle \frac{5}{8}$ |                                                                        |
| Small hoard    | `2`   | $\displaystyle \frac{2}{8}$ |                                                                        |
| Merchant hoard | `4`   |                             | Dropped when merchant is killed                                        |
| Dragon hoard   | `6`   | $\displaystyle \frac{1}{8}$ | Always protected by dragon, can only be picked up when dragon is slain |

-   Spawning
    -   10 piles of gold are spawned per floor
    -   Spawning follows the _common spawning rule_

### Major Items

**Question**. How could you generate items so that the generation of Treasure, Potions, and major items reuses as much code as possible? That is for example, how would you structure your system so that the generation of a potion and then generation of treasure does not duplicate code? How could you reuse the code used to protect both dragon hoards and the Barrier Suit?

#### Compass

-   Denoted by `C`
-   Only appears once per floor
    -   One enemy is the compass holder. When killed, compass is dropped.
-   When picked up by player, stairs become visible

#### Barrier Suit

-   Denoted by `B`
-   Follows same rules as dragon hoard

### Floors

-   Stairs denoted by `\`
    -   Only visible once compass is picked up, otherwise normal floor tile
    -   If a player walks over the stairs without the compass it will still take them to the next level
-   Player and stairs equally likely to spawn in every chamber
    -   Equally likely to spawn in any tile of that chamber
    -   Stairs and players should never spawn in the same chamber

Spawning follows these steps:

1. Player location
2. Stairway location
3. Potions
4. Gold
5. Enemies

-   Multiple objects (enemies, gold, potions) cannot occupy the same cell on the board
    -   Player can walk over gold to collect it
-   Items and enemies should only spawn on a floor tile and never in a doorway, passage, or stairs

### Display

| What                | Symbol    |
| ------------------- | --------- |
| Wall                | `\|`, `-` |
| Doorway             | `+`       |
| Passage             | `#`       |
| Walkable floor tile | `.`       |

Use ASCII colours [gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a](https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a)

### Command Interpreter

| Command(s)                               | Action                                                                                             |
| ---------------------------------------- | -------------------------------------------------------------------------------------------------- |
| `no`, `so`, `ea`, `we`, `nw`, `se`, `sw` | Move player character one block (tile??) in appropriate cardinal direction                         |
| `u <direction>`                          | Uses the potion indicated by the direction (e.g `no`, `so`, `ea`)                                  |
| `a <direction>`                          | Attacks the enemy in the specified direction, if the monster is in the immediately specified block |
| `h, e, d, o`                             | Specified the race the player wants to start the game with                                         |
| `r`                                      | Restart the game                                                                                   |
| `q`                                      | Exit the game                                                                                      |

Board should be re-drawn every time a command is entered.

### Termination

A game ends when:

-   Player reaches stairs on floor 5
-   Player's health reaches 0
-   Player restarts game

-   In first two cases, generate player score
    -   Score is amount of gold

## CLI Options

-   Process single _optional_ command line argument, a file that specifies the layout of each of the 5 floors
    -   File gives EXACT layout as presented to the player
    -   Potions and piles of gold will be specified by numbers instead of being randomly generated

| Number | What                |
| ------ | ------------------- |
| `0`    | Restore health pot. |
| `1`    | Boost atk pot.      |
| `2`    | Boost def pot.      |
| `3`    | Poison health pot.  |
| `4`    | Wound atk pot.      |
| `5`    | Wound def pot.      |
| `6`    | Normal gold pile    |
| `7`    | Small hoard         |
| `8`    | Merchant hoard      |
| `9`    | Dragon hoard        |

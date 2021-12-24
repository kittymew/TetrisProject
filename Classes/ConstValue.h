#pragma once

namespace Constant
{
    extern const int tetrisBlock[7][4][4][4];

    const int down = 80;
    const int left = 75;
    const int right = 77;
    const int up = 72;

    const int mapWidth = 10;
    const int mapHeight = 20;
    const int blockSize = 40;
    const int blockGap = 10;

    const int collision_no = 0;
    const int collision_ground = 1;
    const int collision_lwall = 2;
    const int collision_rwall = 3;
    const int collision_block = 4;
    const int collision_ceiling = 5;
    const int collision_error = -1; // 체크해야할 curBlock이 없다거나 등등

    const float speedDown = 0.7f;
}

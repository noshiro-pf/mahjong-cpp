#ifndef MAHJONG_CPP_BLOCK
#define MAHJONG_CPP_BLOCK

#include <map>
#include <string>
#include <vector>

#include "tile.hpp"

namespace mahjong {

/**
 * @brief ブロックの種類
 */
namespace BlockType {

enum {
    Null   = 0,
    Kotu   = 1,  /* 刻子 */
    Syuntu = 2,  /* 順子 */
    Kantu  = 4,  /* 槓子 */
    Toitu  = 8,  /* 対子 */
    Open   = 16, /* 副露した牌が含まれるかどうか */
    Length = 6,
};

static inline const std::map<int, std::string> Names = {
    {Kotu, "暗刻子"},          {Kotu | Open, "明刻子"},  {Syuntu, "暗順子"},
    {Syuntu | Open, "明順子"}, {Kantu, "暗槓子"},        {Kantu | Open, "明槓子"},
    {Toitu, "暗対子"},         {Toitu | Open, "明対子"},
};

} // namespace BlockType

/**
 * @brief 待ちの種類
 */
namespace WaitType {

enum {
    Null = -1,
    Ryanmen, /* 両面待ち */
    Pentyan, /* 辺張待ち */
    Kantyan, /* 嵌張待ち */
    Syanpon, /* 双ポン待ち */
    Tanki,   /* 単騎待ち */
};

static inline const std::map<int, std::string> Names = {
    {Null, "Null"},        {Ryanmen, "両面待ち"},   {Pentyan, "辺張待ち"},
    {Kantyan, "嵌張待ち"}, {Syanpon, "双ポン待ち"}, {Tanki, "単騎待ち"}};

} // namespace WaitType

/**
 * @brief ブロック
 */
struct Block {
    Block()
        : type(BlockType::Null)
        , min_tile(Tile::Null)
    {
    }

    Block(int type, int min_tile)
        : type(type)
        , min_tile(min_tile)
    {
    }

    /*! ブロックの種類 */
    int type;

    /*! 最小の構成牌 */
    int min_tile;

    std::string to_string() const;
};

/**
 * @brief 文字列に変換する。
 *
 * @return std::string ブロックを表す文字列
 */
inline std::string Block::to_string() const
{
    std::string s;

    s += "[";
    if (type & BlockType::Kotu) {
        for (int i = 0; i < 3; ++i)
            s += Tile::Names.at(min_tile);
    }
    else if (type & BlockType::Syuntu) {
        for (int i = 0; i < 3; ++i)
            s += Tile::Names.at(min_tile + i);
    }
    else if (type & BlockType::Kantu) {
        for (int i = 0; i < 4; ++i)
            s += Tile::Names.at(min_tile);
    }
    else if (type & BlockType::Toitu) {
        for (int i = 0; i < 2; ++i)
            s += Tile::Names.at(min_tile);
    }
    s += fmt::format(", {}]", BlockType::Names.at(type));

    return s;
}

} // namespace mahjong

#endif /* MAHJONG_CPP_BLOCK */

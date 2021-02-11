#include "mahjong/mahjong.hpp"

#include <chrono>

using namespace mahjong;

int main(int, char **)
{
    Hand hand1({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Pinzu5, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6,
                Tile::Sozu6, Tile::Sozu7});
    Hand hand2({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6, Tile::Sozu6,
                Tile::Sozu7, Tile::Pe});
    Hand hand3({Tile::Manzu1, Tile::Manzu1, Tile::Manzu2, Tile::Manzu4, Tile::Manzu5, Tile::Manzu7,
                Tile::Pinzu9, Tile::Sozu3, Tile::Sozu7, Tile::Sozu9, Tile::Ton, Tile::Pe, Tile::Pe,
                Tile::Hatu});
    Hand hand4({Tile::Manzu1, Tile::Manzu2, Tile::Manzu3, Tile::Manzu3, Tile::Manzu4, Tile::Manzu9,
                Tile::Pinzu3, Tile::Pinzu6, Tile::Pinzu8, Tile::Pinzu8, Tile::Sozu1, Tile::Sozu2,
                Tile::Sozu4, Tile::Sozu5});

    int bakaze = Tile::Ton;                 // 場風
    int zikaze = Tile::Ton;                 // 自風
    int turn = 1;                           // 巡目
    int syanten_type = SyantenType::Normal; // 向聴数の種類
    // 考慮する項目
    int flag = ExpectedValueCalculator::CalcSyantenDown   // 向聴戻し考慮
               | ExpectedValueCalculator::CalcTegawari    // 手変わり考慮
               | ExpectedValueCalculator::CalcDoubleReach // ダブル立直考慮
               | ExpectedValueCalculator::CalcIppatu      // 一発考慮
               | ExpectedValueCalculator::CalcHaiteitumo  // 海底撈月考慮
               | ExpectedValueCalculator::CalcUradora;    // 裏ドラ考慮
    std::vector<int> dora_tiles = {Tile::Sya};            // ドラ
    Hand hand = hand4;                                    // 手牌

    ExpectedValueCalculator exp_value_calculator;
    ScoreCalculator score_calculator;

    // 点数計算の設定
    score_calculator.set_bakaze(bakaze);
    score_calculator.set_zikaze(zikaze);
    score_calculator.set_dora_tiles(dora_tiles);

    // 向聴数を計算する。
    auto [_, syanten] = SyantenCalculator::calc(hand, syanten_type);

    // 期待値を計算する。
    auto begin = std::chrono::steady_clock::now();
    auto [success, candidates] =
        exp_value_calculator.calc(hand, score_calculator, syanten_type, flag);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    if (!success) {
        std::cout << "エラー" << std::endl;
        return 1;
    }

    // 期待値が高い順にソートする。
    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        return a.win_probs.front() > b.win_probs.front();
    });

    // 結果を出力する。
    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << fmt::format("手牌: {}, 向聴数: {}, 巡目: {}", hand.to_string(), syanten, turn)
              << std::endl;

    for (const auto &candidate : candidates) {
        std::cout << fmt::format("[打 {}]", Tile::Name.at(candidate.tile)) << " ";

        std::cout << fmt::format(
            "有効牌: {:>2d}種{:>2d}枚, 聴牌確率: {:>5.2f}%, 和了確率: "
            "{:>5.2f}%, 期待値: {:>7.2f} {}",
            candidate.required_tiles.size(), candidate.sum_required_tiles,
            candidate.tenpai_probs[turn - 1] * 100, candidate.win_probs[turn - 1] * 100,
            candidate.exp_values[turn - 1], candidate.syanten_down ? " (向聴戻し)" : "");

        // std::cout << "有効牌";
        // for (const auto [tile, n] : candidate.required_tiles)
        //     std::cout << fmt::format(" {}", Tile::Name.at(tile));
        std::cout << std::endl;

        // std::cout << "巡目ごとの聴牌確率、和了確率、期待値" << std::endl;
        // for (size_t i = 0; i < 17; ++i) {
        //     std::cout << fmt::format("{:<2}巡目 聴牌確率: {:>5.2f}%, 和了確率: "
        //                              "{:>5.2f}%, 期待値: {:.2f}",
        //                              i + 1, candidate.tenpai_probs[i] * 100,
        //                              candidate.win_probs[i] * 100,
        //                              candidate.exp_values[i])
        //               << std::endl;
        // }
    }

    std::cout << fmt::format("計算時間: {}us", elapsed_ms) << std::endl;
}

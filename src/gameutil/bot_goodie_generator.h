#ifndef INCLUDE_BOT_GOODIE_GENERATOR
#define INCLUDE_BOT_GOODIE_GENERATOR

#include <vector>
#include <random>
#include "gametemplate/bot_goodie_template.h"

namespace bot {

class Goodie;

class GoodieGenerator {
public:
    GoodieGenerator(const std::vector<GoodieTemplate>& goodieTemplateLib);

    ~GoodieGenerator()
    {}

    int generate(float prob);

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
    float m_totalWeight;
    int m_maxGoodieIdx;
    std::vector<float> m_weightSums;
};

} // end of namespace bot

#endif

#include "misc/bot_log.h"
#include "gameutil/bot_goodie_generator.h"
#include "app/bot_app.h"

namespace bot {

GoodieGenerator::GoodieGenerator()
    : m_generator(std::random_device()())
    , m_distribution(0.0, 1.0)
{
    init();
}

void GoodieGenerator::init()
{
    const GameLib& lib = App::getInstance().getGameLib();
    const std::vector<GoodieTemplate>& goodieTemplateLib = lib.getGoodieTemplateLib();
    int count = static_cast<int>(goodieTemplateLib.size());

    m_maxGoodieIdx = count - 1;
    m_totalWeight = 0.0;
    m_weightSums.resize(count);
    for (int i = 0; i <= m_maxGoodieIdx; ++i)
    {
        m_totalWeight += goodieTemplateLib[i].getWeight();
        m_weightSums[i] = m_totalWeight;
    }
}

int GoodieGenerator::generate(float prob)
{
    float dice = m_distribution(m_generator);
    if (dice >= prob)
    {
        return -1;
    }

    float s = m_distribution(m_generator) * m_totalWeight;

    if (s < m_weightSums[0])
    {
        LOG_INFO("gen-goodie 0");
        return 0;
    }

    int left = 0, right = m_maxGoodieIdx, mid;

    while (left < right - 1)
    {
        mid = (left + right) >> 1;
        if (s >= m_weightSums[mid])
        {
            left = mid;
        }
        else
        {
            right = mid;
        }
    }

    return right;
}

} // end of namespace bot

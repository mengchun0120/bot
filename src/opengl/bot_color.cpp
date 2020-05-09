#include "misc/bot_log.h"
#include "opengl/bot_color.h"

namespace bot {

bool Color::validateColor(int red, int green, int blue, int alpha)
{
    if (!validateElem(red)) 
    {
        LOG_ERROR("Invalid red value %d", red);
        return false;
    }

    if (!validateElem(green)) 
    {
        LOG_ERROR("Invalid green value %d", green);
        return false;
    }

    if (!validateElem(blue)) 
    {
        LOG_ERROR("Invalid blue value %d", blue);
        return false;
    }

    if (!validateElem(alpha)) 
    {
        LOG_ERROR("Invalid alpha value %d", alpha);
        return false;
    }

    return true;
}

Color::Color()
{
    m_color[0] = 0.0f;
    m_color[1] = 0.0f;
    m_color[2] = 0.0f;
    m_color[3] = 0.0f;
}

bool Color::setColor(int red, int green, int blue, int alpha)
{
    if (!validateColor(red, green, blue, alpha)) 
    {
        return false;
    }

    m_color[0] = red / 255.0f;
    m_color[1] = green / 255.0f;
    m_color[2] = blue / 255.0f;
    m_color[3] = alpha / 255.0f;

    return true;
}

bool Color::setRed(int red)
{
    if (!validateElem(red)) 
    {
        LOG_ERROR("Invalid red value %d", red);
        return false;
    }

    m_color[0] = red / 255.0f;

    return true;
}

bool Color::setRed(float red)
{
    if (!validateElem(red)) 
    {
        LOG_ERROR("Invalid red value %f", red);
        return false;
    }

    m_color[0] = red;

    return true;
}

bool Color::setGreen(int green)
{
    if (!validateElem(green)) 
    {
        LOG_ERROR("Invalid green value %d", green);
        return false;
    }

    m_color[1] = green / 255.0f;

    return true;
}

bool Color::setGreen(float green)
{
    if (!validateElem(green)) 
    {
        LOG_ERROR("Invalid green value %f", green);
        return false;
    }

    m_color[1] = green;

    return true;
}

bool Color::setBlue(int blue)
{
    if (!validateElem(blue)) 
    {
        LOG_ERROR("Invalid blue value %d", blue);
        return false;
    }

    m_color[2] = blue / 255.0f;

    return true;
}

bool Color::setBlue(float blue)
{
    if (!validateElem(blue)) 
    {
        LOG_ERROR("Invalid blue value %f", blue);
        return false;
    }

    m_color[2] = blue;

    return true;
}

bool Color::setAlpha(int alpha)
{
    if (!validateElem(alpha)) 
    {
        LOG_ERROR("Invalid alpha value %d", alpha);
        return false;
    }

    m_color[3] = alpha / 255.0f;
    
    return true;
}

bool Color::setAlpha(float alpha)
{
    if (!validateElem(alpha)) 
    {
        LOG_ERROR("Invalid alpha value %f", alpha);
        return false;
    }

    m_color[3] = alpha;

    return true;
}

} // end of namespace bot

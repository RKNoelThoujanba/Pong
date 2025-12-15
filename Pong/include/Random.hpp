#include <random>

namespace Pong
{
    class RNG
    {
    public:
        RNG()
            :m_Generator(m_Device())
        {
        }

        template<typename T>
        T UniformDistribution(T lo, T hi) //returns x belongs to [lo, hi]
        {
            if constexpr(std::is_same<T, float>() || std::is_same<T, double>())
            {
                return std::uniform_real_distribution<T>(lo, hi)(m_Generator);
            }
            else
            {
                return std::uniform_int_distribution<T>(lo, hi)(m_Generator);
            }
        }
    private:
        std::random_device m_Device;
        std::mt19937_64 m_Generator;
    };
}
#ifndef UTIL_H_
#define UTIL_H_

namespace util
{
	template<typename T1, typename T2, std::size_t N>
	const int GetIndexInBinaryFind(T1(&src)[N], T2&& dest)
	{
		std::size_t i = 0;
		std::size_t j = N - 1;

		while (true)
		{
			if (j < i)
			{
				return -1;
			}
			if (src[i] == dest)
			{
				return i;
			}
			if (i == j)
			{
				return -1;
			}
			if (src[j] == dest)
			{
				return j;
			}

			++i;
			--j;
		}

		return -1;
	}
}

#endif
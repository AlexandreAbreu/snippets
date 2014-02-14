#include <iostream>
#include <string.h>

#include <stdint.h>

// 1 based
template <typename T>
int highest_bit(T value)
{
  int highest = 0;
  if (value == 0)
    {
      return highest;
    }
  static uint8_t masks[] =
    {
      // 0x1
      1,
      // 0x2
      2,
      // 0x3
      2,
      // 0x4
      3,
      // 0x5
      3,
      // 0x6
      3,
      // 0x7
      3,
      // 0x8
      4,
      // 0x9
      4,
      // 0xA
      4,
      // 0xB
      4,
      // 0xC
      4,
      // 0xD
      4,
      // 0xE
      4,
      // 0xF
      4,
    };

  highest = 0;
  unsigned char nib = 0;
  while (value)
    {
      highest += 4;
      nib = (value & 0x0F);
      value >>= 4;
    }

  highest += masks[nib - 1];

  return highest - 4;
}

template <typename T>
std::string
to_binary(const T & value)
{
  std::string binary;
  int highestb = highest_bit(value);
  if (highestb == 0)
    {
      return binary;
    }

  for (int i = 0; i < highestb; ++i)
    {
      binary.append((value & (1 << i)) ? "1" : "0");
    }

  return binary;
}

bool
match(const char * s, const char * p)
{
  const int MASK_BIT_SIZE = 32;

  if (!p || strlen(p) > MASK_BIT_SIZE)
    {
      return false;
    }

  // create the mask bit
  uint32_t masks[(unsigned char)-1] = {0};
  for (int i = 0; i < strlen(p); ++i)
    {
      char c = p[i];
      if (c == '?')
	for (int j = 0; j < sizeof(masks)/sizeof(masks[0]); ++j)
	  masks[j] |= (1 << i);
      masks[c] |= (1 << i);
    }

  uint32_t state = 1;
  bool matches = false;
  for (int i = 0; i < strlen(s); ++i)
    {
      if (masks[s[i]] & state)
	state <<= 1;
      else
	state = 1;
      if (highest_bit(state) == strlen(p))
	{
	  matches = true;
	  break;
	}
    }

  return matches;
}

int main()
{
  std::cout
    << (match("super super super strodsrng super", "strodsrng")
	? "matches!"
	: "no match")
    << std::endl;

  std::cout
    << (match("super super super strodsrng super", "strong")
	? "matches!"
	: "no match")
    << std::endl;

  std::cout
    << (match("super super super strodsrng super", "stro???ng")
	? "matches!"
	: "no match")
    << std::endl;

  return 0;
}


#pragma once

bool pattern_match(const char *str, const char *pattern) 
{
  enum State 
  {
    Exact,    // exact match
    Any,      // ?
    AnyRepeat // *
  };

  const char *s = str;
  const char *p = pattern;
  const char *q = 0;
  int state = 0;

  bool match = true;

  while (match && *p)
  {
    if (*p == '*') 
    {
      state = AnyRepeat;
      q = p+1;
    }
    else if (*p == '?') 
      state = Any;
    else 
      state = Exact;

    if (*s == 0) 
      break;

    switch (state)
    {
    case Exact:
      match = *s == *p;
      s++;
      p++;
      break;

    case Any:
      match = true;
      s++;
      p++;
      break;

    case AnyRepeat:
      match = true;
      s++;

      if (*s == *q) p++;
      break;
    }
  }

  if (state == AnyRepeat) return (*s == *q);
  else if (state == Any) return (*s == *p);
  else return match && (*s == *p);
} 

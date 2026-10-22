# miscellaneous
Miscellaneous programs and cool tidbits. <br/>

# setup

```
source setup_venv.sh
```

# usage

For header files, if you find things to be unwieldy then the simplest solution is to wrap all the code content past the other imports in a giant namespace. For example, `hilbert.h`:

```
#ifndef HILBERT_H_NORMALEXISTING
#define HILBERT_H_NORMALEXISTING

#include <array>
/* ... more imports */

namespace HILBERT {

typedef uint32_t cellType;
typedef std::vector<std::vector<cellType>> hilbertMatrix;

/* ... rest of the code */

} // End the namespace

#endif
```
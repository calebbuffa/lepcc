# lepcc

```python
import lepcc
import numpy as np

ctx = lepcc.Context()
with open("path/to/xyz.bin", "rb") as file:
    buffer = lepcc.decode_xyz(file.read())
    xyz = np.frombuffer(buffer, np.float64).reshape(-1, 3)
```

`lepcc` is a Python wrapper for Esri's [LEPCC compression library](https://github.com/Esri/lepcc).

## Installation

`pip install https://github.com/calebbuffa/lepcc.git`

or

```bash
git clone --recurse-submodules https://github.com/calebbuffa/lepcc.git
cd lepcc
pip install .
```

Note: Currently only supports Windows.
